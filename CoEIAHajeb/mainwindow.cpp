/*
* Copyright (C) 2014 - Center of Excellence in Information Assurance
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <QtGui>
#include "mainwindow.h"
#include "websitelockerwidget.h"
#include "programlockerwidget.h"
#include "logswidget.h"
#include "screenshotwidget.h"
#include "settingswidget.h"
#include "browsetimewidget.h"
#include "homewidget.h"
#include "SlidingStackedWidget.h"
#include "applicationstatus.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{    
    HomeWidget* homeWidget = new HomeWidget;
    SettingsWidget* settingsWidget = new SettingsWidget;
    LogsWidget* logsWidget = new LogsWidget;
    ScreenshotWidget* screenshotWidget = new ScreenshotWidget;
    BrowseTimeWidget* browseTimeWidget = new BrowseTimeWidget;
    WebsiteLockerWidget* websiteLockerWidget = new WebsiteLockerWidget;
    ProgramLockerWidget* programLockerWidget = new ProgramLockerWidget;

    currentPageLabel = new QLabel;
    versionLabel = new QLabel;
    versionLabel->setObjectName("versionLabel");

    connect(homeWidget,SIGNAL(updateModel()),settingsWidget,SLOT(updateUsersModel()));
    connect(homeWidget,SIGNAL(updateModel()),logsWidget,SLOT(updateUsersModel()));
    connect(homeWidget,SIGNAL(updateModel()),screenshotWidget,SLOT(updateUsersModel()));
    connect(homeWidget,SIGNAL(updateModel()),browseTimeWidget,SLOT(updateUsersModel()));
    connect(homeWidget,SIGNAL(updateModel()),websiteLockerWidget,SLOT(updateUsersModel()));
    connect(homeWidget,SIGNAL(updateModel()),programLockerWidget,SLOT(updateUsersModel()));

    connect(homeWidget,SIGNAL(registerd()),this,SLOT(readAvailableDays()));

    connect(this,SIGNAL(updateLogs()),logsWidget,SLOT(updateLog()));
    connect(this,SIGNAL(updateScreenshots()),screenshotWidget,SLOT(updateScreenshot()));
    connect(this,SIGNAL(updateStatistics()),settingsWidget,SLOT(updateStatistics()));

    stackedWidget = new SlidingStackedWidget(this);
    stackedWidget->addWidget(homeWidget);
    stackedWidget->addWidget(settingsWidget);
    stackedWidget->addWidget(logsWidget);
    stackedWidget->addWidget(screenshotWidget);
    stackedWidget->addWidget(browseTimeWidget);
    stackedWidget->addWidget(websiteLockerWidget);
    stackedWidget->addWidget(programLockerWidget);
    stackedWidget->setSpeed(500);

    this->setCentralWidget(stackedWidget);
    createDockWidget();
    readAvailableDays();
    createStatusBar();

    this->setWindowTitle("نظام حاجب");
    this->setWindowIcon((QIcon(":/images/hajibIcon.ico")));
    this->setFixedWidth(800);
    this->setFixedHeight(600);

    this->setCurrentWindow(0);
}

MainWindow::~MainWindow()
{

}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPixmap pixmap(":/images/hajeb-bg.png");
    QPainter p(this);
    p.drawPixmap(0,0,pixmap);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    emit updateCurrentDateTime();
    event->accept();
}

void MainWindow::createDockWidget() {
    signalMapper = new QSignalMapper(this);

    // Header Widget
    QWidget* headerWidget = new QWidget;
    headerWidget->setFixedHeight(109);

    QDockWidget* headerDockWidget = new QDockWidget(this);
    headerDockWidget->setObjectName("headerDockWidget");
    headerDockWidget->setFloating(false);
    headerDockWidget->setTitleBarWidget(new QWidget);
    headerDockWidget->setWidget(headerWidget);

    this->addDockWidget(Qt::TopDockWidgetArea,headerDockWidget);

    // Right Panel Widget
    rightPanelWidget = new QWidget;
    rightPanelWidget->setObjectName("rightPanelWidget");
    rightPanelWidget->setFixedWidth(213);
    rightPanelWidget->setFixedHeight(481);
    QDockWidget* rightDockWidget = new QDockWidget(this);
    rightDockWidget->setObjectName("rightDockWidget");
    rightDockWidget->setFloating(false);
    rightDockWidget->setTitleBarWidget(new QWidget);
    rightDockWidget->setWidget(rightPanelWidget);

    this->addDockWidget(Qt::RightDockWidgetArea,rightDockWidget);

    // Set geometry for each button at fixed place.
    int y_aix = 8;
    int x_aix = 15;
    int button_height = 58;
    int button_width = 183;
    int spacing = 1 ;

    QPushButton* homeButton = new QPushButton(rightPanelWidget);
    homeButton->setObjectName("0_toolButton");
    homeButton->setGeometry(x_aix,y_aix,button_width,button_height);
    homeButton->setStyleSheet("QPushButton {background-image: url(:/images/home.png);background-color: transparent;} QPushButton:hover {background-image: url(:/images/home-hover.png);background-color: transparent;} QPushButton:pressed,QPushButton:checked {background-image: url(:/images/home-checked.png);background-color: transparent;}");
    homeButton->setCheckable(true);
    homeButton->setChecked(true);
    homeButton->setToolTip("الرئيسية");
    homeButton->setStatusTip("البداية لحاجب ويمكنك من خلالها التحكم في عمل خدمات حاحب على كل المستخدمين بالاضافة الى خيارات اخرى ...");
    connect(homeButton,SIGNAL(clicked()),signalMapper,SLOT(map()));
    signalMapper->setMapping(homeButton,Home);

    y_aix += button_height + spacing;
    QPushButton* userSettingsButton = new QPushButton(rightPanelWidget);
    userSettingsButton->setObjectName("1_toolButton");
    userSettingsButton->setGeometry(x_aix,y_aix,button_width,button_height);
    userSettingsButton->setStyleSheet("QPushButton {background-image: url(:/images/userSettings.png);background-color: transparent;} QPushButton:hover {background-image: url(:/images/userSettings-hover.png);background-color: transparent;} QPushButton:pressed,QPushButton:checked {background-image: url(:/images/userSettings-checked.png);background-color: transparent;}");
    userSettingsButton->setCheckable(true);
    userSettingsButton->setToolTip("إعدادات المستخدم");
    userSettingsButton->setStatusTip("إعدادات المستخدم تمكنك من تفعيل او تعطيل خدمات حاجب لمستخدم ما ...");
    connect(userSettingsButton,SIGNAL(clicked()),signalMapper,SLOT(map()));
    signalMapper->setMapping(userSettingsButton,UserSettings);

    y_aix += button_height + spacing;
    QPushButton* serviceLogButton = new QPushButton(rightPanelWidget);
    serviceLogButton->setObjectName("2_toolButton");
    serviceLogButton->setGeometry(x_aix,y_aix,button_width,button_height);
    serviceLogButton->setStyleSheet("QPushButton {background-image: url(:/images/userLogs.png);background-color: transparent;} QPushButton:hover {background-image: url(:/images/userLogs-hover.png);background-color: transparent;} QPushButton:pressed,QPushButton:checked {background-image: url(:/images/userLogs-checked.png);background-color: transparent;}");
    serviceLogButton->setCheckable(true);
    serviceLogButton->setToolTip("سجلات المستخدمين");
    serviceLogButton->setStatusTip("من خلال السجلات يمكنك متابعة كافة الأحداث لكل المستخدمين ...");
    connect(serviceLogButton,SIGNAL(clicked()),signalMapper,SLOT(map()));
    signalMapper->setMapping(serviceLogButton,ServiceLog);

    y_aix += button_height + spacing;
    QPushButton* screenshotMonitorButton = new QPushButton(rightPanelWidget);
    screenshotMonitorButton->setObjectName("3_toolButton");
    screenshotMonitorButton->setGeometry(x_aix,y_aix,button_width,button_height);
    screenshotMonitorButton->setStyleSheet("QPushButton {background-image: url(:/images/screenshotsMonitor.png);background-color: transparent;} QPushButton:hover {background-image: url(:/images/screenshotsMonitor-hover.png);background-color: transparent;} QPushButton:pressed,QPushButton:checked {background-image: url(:/images/screenshotsMonitor-checked.png);background-color: transparent;}");
    screenshotMonitorButton->setCheckable(true);
    screenshotMonitorButton->setToolTip("مراقبة الشاشة");
    screenshotMonitorButton->setStatusTip("مشاهدة لقطات الشاشة لكل المستخدمين والتحكم بوقت أخذ اللقطات ...");
    connect(screenshotMonitorButton,SIGNAL(clicked()),signalMapper,SLOT(map()));
    signalMapper->setMapping(screenshotMonitorButton,ScreenshotMonitor);

    y_aix += button_height + spacing;
    QPushButton* surfeTimeButton = new QPushButton(rightPanelWidget);
    surfeTimeButton->setObjectName("4_toolButton");
    surfeTimeButton->setGeometry(x_aix,y_aix,button_width,button_height);
    surfeTimeButton->setStyleSheet("QPushButton {background-image: url(:/images/browseTime.png);background-color: transparent;} QPushButton:hover {background-image: url(:/images/browseTime-hover.png);background-color: transparent;} QPushButton:pressed,QPushButton:checked {background-image: url(:/images/browseTime-checked.png);background-color: transparent;}");
    surfeTimeButton->setCheckable(true);
    surfeTimeButton->setToolTip("زمن التصفح");
    surfeTimeButton->setStatusTip("تحديد وقت تصفح الاتنرنت لكل مستخدم لكافة أيام الأسبوع ...");
    connect(surfeTimeButton,SIGNAL(clicked()),signalMapper,SLOT(map()));
    signalMapper->setMapping(surfeTimeButton,SurfTime);

    y_aix += button_height + spacing;
    QPushButton* websiteLockButton = new QPushButton(rightPanelWidget);
    websiteLockButton->setObjectName("5_toolButton");
    websiteLockButton->setGeometry(x_aix,y_aix,button_width,button_height);
    websiteLockButton->setStyleSheet("QPushButton {background-image: url(:/images/websiteLock.png);background-color: transparent;} QPushButton:hover {background-image: url(:/images/websiteLock-hover.png);background-color: transparent;} QPushButton:pressed,QPushButton:checked {background-image: url(:/images/websiteLock-checked.png);background-color: transparent;}");
    websiteLockButton->setCheckable(true);
    websiteLockButton->setToolTip("حظر المواقع");
    websiteLockButton->setStatusTip("اضافة وازالة موقع من قائمة المواقع المحظورة ...");
    connect(websiteLockButton,SIGNAL(clicked()),signalMapper,SLOT(map()));
    signalMapper->setMapping(websiteLockButton,WebsiteLocker);

    y_aix += button_height + spacing;
    QPushButton* programLockButton = new QPushButton(rightPanelWidget);
    programLockButton->setObjectName("6_toolButton");
    programLockButton->setGeometry(x_aix,y_aix,button_width,button_height);
    programLockButton->setStyleSheet("QPushButton {background-image: url(:/images/programLock.png);background-color: transparent;} QPushButton:hover {background-image: url(:/images/programLock-hover.png);background-color: transparent;} QPushButton:pressed,QPushButton:checked {background-image: url(:/images/programLock-checked.png);background-color: transparent;}");
    programLockButton->setCheckable(true);
    programLockButton->setToolTip("حظر البرامج");
    programLockButton->setStatusTip("اضافة وازالة برنامج من قائمة البرامج المحظورة ...");
    connect(programLockButton,SIGNAL(clicked()),signalMapper,SLOT(map()));
    signalMapper->setMapping(programLockButton,ProgramLocker);

    connect(signalMapper,SIGNAL(mapped(int)),this,SLOT(setCurrentWindow(int)));
}

void MainWindow::createStatusBar() {
    this->statusBar()->showMessage("مرحبا بك في نظام حاجب");
    this->statusBar()->addPermanentWidget(versionLabel);
}

void MainWindow::readAvailableDays() {
    int availableDays;
    if (ApplicationStatus::isCleanVersion()) {
        versionLabel->setText("");
        return;
    }
    QDateTime installDate = ApplicationStatus::installDate();
    QDateTime endDate = installDate.addDays(ApplicationStatus::ALLOWED_DAYS);
    availableDays = QDateTime::currentDateTime().daysTo(endDate);
    qDebug() << availableDays;
    versionLabel->setText(QString("النسخة المجانية: تبقى %1 يوم").arg(availableDays));
}

void MainWindow::setCurrentWindow(int id) {
    QString buttonToolTip ;
    QString buttonName = QString::number(id) + "_toolButton";
    QList<QPushButton*> buttons = rightPanelWidget->findChildren<QPushButton*>();
    foreach (QPushButton* button,buttons) {
        if (button->objectName() == buttonName) {
            button->setChecked(true);
            buttonToolTip = button->toolTip();
        }
        else
            button->setChecked(false);
    }

    stackedWidget->slideInIdx(id);

    // Update widget data.
    switch (id) {
    case ServiceLog:
        emit updateLogs();
        break;
    case ScreenshotMonitor:
        emit updateScreenshots();
        break;
    case UserSettings:
        emit updateStatistics();
    default:
        break;
    }

    currentPageLabel->setText("<font color=gray>"+buttonToolTip+"</font>");
    this->statusBar()->addPermanentWidget(currentPageLabel);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    qDebug() << event->size().width() << "*" << event->size().height();
}
