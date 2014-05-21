#include <QtGui>
#include <QPropertyAnimation>
#include <QGraphicsEffect>
#include <QtSql/QtSql>
#include <QDebug>
#include "widget.h"
#include "ui_widget.h"
#include "../Global/global.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent,Qt::Dialog),ui(new Ui::Widget)
{
    ui->setupUi(this);

    updateClock();

    QList<QToolButton*> buttonList = ui->timesGroupBox->findChildren<QToolButton*>();
    foreach (QToolButton* button,buttonList) {
        button->setEnabled(false);
    }

    ui->closeButton->setEnabled(true);

    previousTime = 0;
    previousHour = 0;
    previousMin = 0;
    isCounterStopped = false;
    ui->startButton->setChecked(true);

    moveWidgetToBottom();
    createActions();
    createMenus();
    createTryIcon();

    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(readUserTimeTable()));
    m_timer->start();

    m_showTimer = new QTimer(this);
    m_showTimer->setInterval(50);
    connect(m_showTimer,SIGNAL(timeout()),this,SLOT(increaseOpacityWidget()));

    m_opcaity = 0 / 10 ;
    m_settingsDisabled = false;

    QTimer::singleShot(10,this,SLOT(hide()));
}

Widget::~Widget()
{
}

void Widget::closeEvent(QCloseEvent* event) {
    if (trayIcon->isVisible()) {
        hide();
        event->ignore();
    }
}

void Widget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
}

void Widget::showEvent(QShowEvent *event) {
    this->setWindowOpacity((float)m_opcaity / 10.0);
    this->repaint();
    if (!m_showTimer->isActive())
        m_showTimer->start();
}

void Widget::hideEvent(QHideEvent *event) {

}

void Widget::increaseOpacityWidget() {
    m_opcaity ++ ;
    this->setWindowOpacity((float)m_opcaity / 10.0);
    if (m_opcaity >= 10) {
        m_showTimer->stop();
        m_opcaity = 0 / 10;
    }
}

void Widget::decreaseOpacityWidget() {

}

void Widget::createActions() {
    maximizeAct = new QAction("تكبير",this);
    connect(maximizeAct,SIGNAL(triggered()),this,SLOT(showClicked()));

    minimizeAct = new QAction("تصغير",this);
    connect(minimizeAct,SIGNAL(triggered()),this,SLOT(hide()));

    aboutAct = new QAction("عن حاجب",this);
    connect(aboutAct,SIGNAL(triggered()),this,SLOT(showMessage()));
}

void Widget::createMenus() {
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(maximizeAct);
    trayIconMenu->addAction(minimizeAct);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(aboutAct);
}

void Widget::createTryIcon() {
    trayIcon = new QSystemTrayIcon(this);
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    trayIcon->setToolTip("برنامج حاجب - مركز التميز لأمن المعلومات");
    trayIcon->setIcon(QIcon(":/images/logo.PNG"));
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->show();
}

void Widget::showMessage() {
    trayIcon->showMessage("نظام حاجب - مركز التميز لأمن المعلومات","خدمة التحكم بزمن التصفح تعمل.");
}

void Widget::iconActivated(QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
    case QSystemTrayIcon::DoubleClick:
        if (!this->isVisible())
            showNormal();
        else
            hide();
        break;
    default:
        ;
    }
}

void Widget::showClicked() {
    this->show();
    this->raise();
    this->activateWindow();
}

void Widget::readUserTimeTable() {

    qApp->processEvents();

    int userId;

    if (QSysInfo::windowsVersion() == QSysInfo::WV_VISTA || QSysInfo::windowsVersion() == QSysInfo::WV_WINDOWS7) {
        QString userName = getCurrentUserName();
        userId = getCurrentUserId(userName);
    } else {
        userId = getCurrentUserId();
    }

    updateClock();

    // Check if current user is controlled by Hajeb
    if (userId == -1) {
        showUserNotControllWidget();
        return ;
    }

    // Check if BrowseTime monitor is not enabled for this user.
    if (!isEnabledForUser(userId)) {
        showMonitorIsDisabled();
        return ;
    }

    // Check browseTime Method.
    if (userMethod(userId) == 0) {
        timeTableMethod(userId);
    } else {
        counterMethod(userId);
    }
}

int Widget::currentDate() {
    int day = QDate::currentDate().dayOfWeek();
    int dayNo = day + 2;
    if (dayNo > 7)
        dayNo = (dayNo % 8) + 1;

    return dayNo;
}

int Widget::readTodayTime(int id) {
    int timeTable;
    int dayNo = currentDate();

    QSqlQuery query;
    query.prepare("SELECT timetable FROM BrowseTimesTable WHERE userId = ? AND dayNo = ?");
    query.addBindValue(id);
    query.addBindValue(dayNo);
    query.exec();
    query.next();
    timeTable = query.value(0).toInt();

    return timeTable;
}

QTime Widget::readTodayRemaingingCounter(int id) {
    int dayNo = currentDate();
    QSqlQuery query;
    query.prepare("SELECT remainingTime FROM BrowseTimesCounterTable WHERE userId = ? AND dayNo = ?");
    query.addBindValue(id);
    query.addBindValue(dayNo);
    query.exec();
    query.next();
    return query.value(0).toTime();
}

void Widget::writeTodayRemainingCounter(int id,QTime time) {
    int dayNo = currentDate();
    QSqlQuery query;
    query.prepare("UPDATE BrowseTimesCounterTable SET remainingTime = ?  WHERE userId = ? AND dayNo = ?");
    query.addBindValue(time);
    query.addBindValue(id);
    query.addBindValue(dayNo);
    query.exec();
}

QTime Widget::readTodayAllowedCounter(int id) {
    int dayNo = currentDate();
    QSqlQuery query;
    query.prepare("SELECT allowedTime FROM BrowseTimesCounterTable WHERE userId = ? AND dayNo = ?");
    query.addBindValue(id);
    query.addBindValue(dayNo);
    query.exec();
    query.next();
    return query.value(0).toTime();
}



QDate Widget::readCounterLastDate(int id) {
    int dayNo = currentDate();
    QSqlQuery query;
    query.prepare("SELECT lastUseDate FROM BrowseTimesCounterTable WHERE userId = ? AND dayNo = ?");
    query.addBindValue(id);
    query.addBindValue(dayNo);
    query.exec();
    query.next();
    return query.value(0).toDate();
}

void Widget::writeCounterLastDate(int id) {
    int dayNo = currentDate();
    QSqlQuery query;
    query.prepare("UPDATE BrowseTimesCounterTable SET lastUseDate = ? WHERE userId = ? AND dayNo = ?");
    query.addBindValue(QDate::currentDate());
    query.addBindValue(id);
    query.addBindValue(dayNo);
    query.exec();
}


QString Widget::getCurrentUserName(int userId) {
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT name from UsersTable WHERE userId = ?");
    query.addBindValue(userId);
    query.exec();
    query.next();
    return query.value(0).toString();
}

bool Widget::isEnabledForUser(int id) {
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT browseTime from SettingsTable WHERE userId = ?");
    query.addBindValue(id);
    query.exec();
    query.next();
    return (query.value(0).toInt()?true:false);
}

int Widget::userMethod(int id) {
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT browseTimeMethod from SettingsTable WHERE userId = ?");
    query.addBindValue(id);
    query.exec();
    query.next();
    return query.value(0).toInt();
}

int Widget::getCurrentUserId() {
    int userId;
    QFile file(USER_ID_FILE_PATH);
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    stream >> userId;
    file.close();
    return userId;
}

void Widget::on_closeButton_clicked() {
    this->hide();
}

void Widget::moveWidgetToBottom() {
    QDesktopWidget* desktop=QApplication::desktop();
    qDebug() << desktop->screenCount();
    int screenWidth=desktop->width() / desktop->screenCount();
    int screenHeight=desktop->height();
    qDebug() << screenWidth;
    qDebug() << screenHeight;
    this->setFixedSize(QSize(this->width(),this->height()));
    this->move(screenWidth-width(),screenHeight-height()-50);
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::ToolTip);
    this->setAttribute(Qt::WA_TranslucentBackground);
}

void Widget::updateClock() {
    ui->hourLabel->setText(QString::number(QTime::currentTime().hour())+":"+QString::number(QTime::currentTime().minute())+":"+QString::number(QTime::currentTime().second()));
}

void Widget::showUserNotControllWidget() {
    ui->stackedWidget->setCurrentIndex(0);
    m_settingsDisabled = true;
    QList<QToolButton*> buttonList = ui->timesGroupBox->findChildren<QToolButton*>();
    foreach (QToolButton* button,buttonList) {
        button->hide();
    }
    QList<QLabel*> labelList = ui->timesGroupBox->findChildren<QLabel*>();
    foreach (QLabel* label,labelList) {
        label->hide();
    }
    ui->hajebIconLabel->show();
    ui->hajebTitleLablel->show();
    ui->hourLabel->show();
    ui->hourStringLabel->show();
    ui->closeButton->show();
    ui->label->show();
    m_settingsDisabled = true;

    ui->label->setText("نظام حاجب لا يعمل على هذا المستخدم");
}

void Widget::showMonitorIsDisabled() {
    ui->stackedWidget->setCurrentIndex(0);
    qDebug() << "BrowseTime is disabled for this account.";
    QList<QToolButton*> buttonList = ui->timesGroupBox->findChildren<QToolButton*>();
    foreach (QToolButton* button,buttonList) {
        button->hide();
    }
    QList<QLabel*> labelList = ui->timesGroupBox->findChildren<QLabel*>();
    foreach (QLabel* label,labelList) {
        label->hide();
    }
    ui->hajebIconLabel->show();
    ui->hajebTitleLablel->show();
    ui->hourLabel->show();
    ui->hourStringLabel->show();
    ui->closeButton->show();
    ui->label->show();
    m_settingsDisabled = true;

    ui->label->setText("خدمة الانترنت غير محدودة");
}

void Widget::showTimeTableWidget() {
    ui->stackedWidget->setCurrentIndex(0);
    QList<QToolButton*> buttonList = ui->timesGroupBox->findChildren<QToolButton*>();
    foreach (QToolButton* button,buttonList) {
        button->show();
    }
    QList<QLabel*> labelList = ui->timesGroupBox->findChildren<QLabel*>();
    foreach (QLabel* label,labelList) {
        label->show();
    }
    ui->label->setText("الساعات المسموح فيها باستخدام الإنترنت اليوم");
    m_settingsDisabled = false;
}

void Widget::showCounterWidget() {
    ui->stackedWidget->setCurrentIndex(1);
    ui->hourLcdNumber->show();
    ui->minLcdNumber->show();
    ui->secLcdNumber->show();
    ui->startButton->show();
    QList<QLabel*> labelList = ui->timesGroupBox->findChildren<QLabel*>();
    foreach (QLabel* label,labelList) {
        label->show();
    }
    ui->label->setText("عدد الساعات المسموح فيها باستخدام الإنترنت اليوم");
    m_settingsDisabled = false;
}

void Widget::updateTimeTable(int todayTiming) {
    ui->stackedWidget->setCurrentIndex(0);
    for (int i=0;i<24;++i) {
        QString buttonName = "button_1_" + QString::number(i);
        int bitPos = 0x00000001;
        bitPos <<= i;
        ui->timesGroupBox->findChild<QToolButton*>(buttonName)->setChecked(todayTiming & bitPos);
    }
    previousTime = todayTiming;
    qDebug() << "GUI updated";
}

void Widget::updateCounter(int hour,int min,int sec) {
    ui->stackedWidget->setCurrentIndex(1);
    ui->hourLcdNumber->display(hour);
    ui->minLcdNumber->display(min);
    ui->secLcdNumber->display(sec);
    previousHour = hour;
    previousMin = min;
    qDebug() << "GUI updated";
}

int Widget::readNetCmd() {
    int cmd = 0;
    QFile file(CONNECTCMD_FILE_PATH);
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    stream >> cmd;
    file.close();
    return cmd;
}

void Widget::writeNetCmd(int cmd) {
    QFile file(CONNECTCMD_FILE_PATH);
    file.open(QIODevice::WriteOnly|QIODevice::Truncate);
    QTextStream stream(&file);
    stream << cmd ;
    file.close();
}

void Widget::timeTableMethod(int userId) {
    ui->stackedWidget->setCurrentIndex(0);
    int todayTiming = readTodayTime(userId);

    // reshow hole GUI
    if (m_settingsDisabled) {
        showTimeTableWidget();
    }

    // Update GUI
    if (previousTime != todayTiming) {
        updateTimeTable(todayTiming);
    }

    // get current hour and compare it with todayTiming and check if it is enable/disable.
    // if it enable write 1 on file else 0.

    int bitPos = 0x00000001;
    bitPos <<= QTime::currentTime().hour();

//    int cmd = readNetCmd();
//    qDebug() << "cmd: " << cmd;

    if (todayTiming & bitPos) {
        // Enable
        qDebug() << "Internet allow now";
        writeNetCmd(1);

    } else {
        // Disable
        qDebug() << "Internet not allow now";
        writeNetCmd(0);
    }
}

void Widget::counterMethod(int userId) {
    ui->stackedWidget->setCurrentIndex(1);
    QTime todayRemainingCounter = readTodayRemaingingCounter(userId);
    qDebug() << "RemainingTime: " << todayRemainingCounter;
    ui->hourLcdNumber->display(todayRemainingCounter.hour());
    ui->minLcdNumber->display(todayRemainingCounter.minute());
    ui->secLcdNumber->display(todayRemainingCounter.second());

    QTime todayAllowedCounter = readTodayAllowedCounter(userId);
    QDate lastUseDate = readCounterLastDate(userId);

    // reshow hole GUI
    if (m_settingsDisabled) {
        showCounterWidget();
    }

    if (lastUseDate != QDate::currentDate()) {
        // update date and remainingTime.
        writeTodayRemainingCounter(userId,todayAllowedCounter);
        writeCounterLastDate(userId);
        return ;
    }


    if (isCounterStopped) {
        // disconnect net
        writeNetCmd(0);
        qDebug() << "Internt is disconnected";
        return ;
    }

    if (todayRemainingCounter == QTime(0,0,0)) {
        // disconnect net
        writeNetCmd(0);
        qDebug() << "Internt is disconnected";
        return;
    }

    writeNetCmd(1);
    qDebug() << "Internt is connected";
    qDebug() << "Remaining Time: " << todayRemainingCounter;

    // update remaining time.
    todayRemainingCounter = todayRemainingCounter.addSecs(-1);
    updateCounter(todayRemainingCounter.hour(),todayRemainingCounter.minute(),todayRemainingCounter.second());
    writeTodayRemainingCounter(userId,todayRemainingCounter);


}

void Widget::on_startButton_toggled(bool checked) {
    if (checked) {
        isCounterStopped = false;
        ui->startButton->setText("Disconnect");
        ui->startButton->setIcon(QIcon(":/images/pause.png"));
        qDebug() << "start ..";
    } else {
        isCounterStopped = true;
        ui->startButton->setText("Connect");
        ui->startButton->setIcon(QIcon(":/images/start.png"));
        qDebug() << "stop ..";
    }
}

int Widget::getCurrentUserId(QString userName) {
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT userId from UsersTable WHERE name = ?");
    query.addBindValue(userName.trimmed());
    query.exec();
    if (query.next())
        return query.value(0).toInt();
    else
        return -1;
}

QString Widget::getCurrentUserName() {
    QProcess* process = new QProcess;
    process->start("CoEIAUserNameWin7.exe");
    process->waitForStarted();
    process->waitForFinished();
    QString output = process->readAll();
    delete process;
    QStringList lines = output.split("\n");
    QString userName = lines.at(0);
    return userName;
}
