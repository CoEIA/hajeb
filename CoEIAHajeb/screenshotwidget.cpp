#include <QSqlTableModel>
#include <QSqlQuery>
#include <QListWidgetItem>
#include <QPixmap>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include "screenshotwidget.h"
#include "ui_screenshotwidget.h"
#include "../Global/global.h"
#include "useraccounts.h"

ScreenshotWidget::ScreenshotWidget(QWidget *parent):
    QWidget(parent),ui(new Ui::ScreenshotWidget),MAX_IMG(6)
{
    ui->setupUi(this);
    screenshotTitle = "صور لقطات الشاشة للمستخدم ";
    ui->screenshotGroupBox->setTitle(screenshotTitle);

    m_totalPages = 1;
    m_currentPage = 1;

    createModel();
    initListWidget();

    connect(this,SIGNAL(pageChanged(int)),this,SLOT(updateImageListView(int)));

    int index = ui->userNameComboBox->findText(getCurrentUserName());
    if (index == -1)
        index = 0 ;

    ui->userNameComboBox->setCurrentIndex(index);
    readUserScreenshot(index);
    readUserScreenshotTime(index);
}

void ScreenshotWidget::createModel() {
    // Load model to userNameComboBox
    userNameComboBoxModel = new QSqlTableModel(this,QSqlDatabase::database(PCSDB_NAME));
    userNameComboBoxModel->setTable("UsersTable");
    userNameComboBoxModel->select();
    ui->userNameComboBox->setModel(userNameComboBoxModel);
    ui->userNameComboBox->setModelColumn(1);
    connect(ui->userNameComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(readUserScreenshot(int)));
    connect(ui->userNameComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(readUserScreenshotTime(int)));
}

void ScreenshotWidget::initListWidget() {
    ui->listWidget->setIconSize(QSize(150,150));
}

void ScreenshotWidget::on_updateScreenshotButton_clicked() {
    updateScreenshot();
    QMessageBox::information(this,"تم تحديث اللقطات",QString("تم تحديث لقطات الشاشة للمستخدم %1.").arg(ui->userNameComboBox->currentText()));
}

void ScreenshotWidget::updateScreenshot() {
    readUserScreenshot(ui->userNameComboBox->currentIndex());
}

void ScreenshotWidget::on_clearScreenshotButton_clicked() {
    QString userName = ui->userNameComboBox->currentText();
    QString path = INSTALL_PATH + "\\Screenshots\\" + userName;
    QDir dir(path);
    foreach (QFileInfo fileInfo,dir.entryInfoList(QDir::Files)) {
        qDebug() << "remove " << fileInfo.fileName() << " : " << dir.remove(fileInfo.fileName());
    }
    ui->listWidget->clear();
    imageList.clear();
    QMessageBox::information(this,"تم الحذف",QString("تم حذف لقطات الشاشة للمستخدم %1 بشكل صحيح").arg(userName));
    setCurrentPage(1);
    m_totalPages = 1;
}

void ScreenshotWidget::readUserScreenshot(int index) {
    // update groupbox title
    QString title = screenshotTitle + ui->userNameComboBox->currentText() + " ";
    ui->screenshotGroupBox->setTitle(title);

    if (index < 0)
        return ;

    QString userName = ui->userNameComboBox->model()->index(index,1).data().toString();
    QString imagePath = SCREENSHOTS_PATH;
    QDir userNameDir(imagePath);
    if (!userNameDir.exists(imagePath + "\\" + userName))
        userNameDir.mkdir(userName);

    userNameDir.cd(userName);

    // read all images.
    imageList.clear();
    imageList = userNameDir.entryInfoList(QDir::Files,QDir::Time);

    m_totalPages = (imageList.count() / MAX_IMG) +1 ;
    if ((imageList.count() % MAX_IMG) == 0 && imageList.count())
        m_totalPages--;

    ui->totalPageLCD->display(m_totalPages);
    setCurrentPage(1);
}

void ScreenshotWidget::setCurrentPage(int page) {
    m_currentPage = page ;
    emit pageChanged(page);
    ui->currentPageLCD->display(m_currentPage);
}

QFileInfoList ScreenshotWidget::getPageImage(int pageNo) {
    QFileInfoList list;
    int idx = (pageNo-1) * MAX_IMG;
    int limit = idx+MAX_IMG;
    int totalImageSize = imageList.count();
    if (totalImageSize < MAX_IMG || totalImageSize < limit )
        limit = totalImageSize;

    for (int i=idx; i<limit; ++i)
        list << imageList[i];
    return list;
}

void ScreenshotWidget::updateImageListView(int pageNo) {
    ui->listWidget->clear();
    QFileInfoList imageList = getPageImage(pageNo);
    QString fileName;
    QString filePath;
    for (int i=0; i<imageList.size(); ++i) {
        fileName = imageList.at(i).fileName();
        filePath = imageList.at(i).absoluteFilePath();
        QPixmap pixmap(filePath);
        QListWidgetItem* item = new QListWidgetItem(QIcon(pixmap),fileName);
        ui->listWidget->insertItem(i,item);
    }
}

void ScreenshotWidget::readUserScreenshotTime(int index) {
    int userId = ui->userNameComboBox->model()->index(index,0).data().toInt();
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    query.prepare("SELECT timePeriod FROM ScreenshotsTable WHERE userId = ?");
    query.addBindValue(userId);
    query.exec();
    if (query.next())
        ui->spinBox->setValue(query.value(0).toInt());
}

void ScreenshotWidget::on_saveButton_clicked() {
    int userId = ui->userNameComboBox->model()->index(ui->userNameComboBox->currentIndex(),0).data().toInt();
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    query.prepare("UPDATE ScreenshotsTable SET timePeriod = ? WHERE userId = ?");
    query.addBindValue(ui->spinBox->value());
    query.addBindValue(userId);
    query.exec();
    QMessageBox::information(this,"تم تحديث التوقيت",QString("تم تحديث وقت لقطات الشاشة للمستخدم %1.").arg(ui->userNameComboBox->currentText()));

}

void ScreenshotWidget::updateUsersModel() {
    int idx = ui->userNameComboBox->currentIndex();
    userNameComboBoxModel->select();
    if (idx < 0 || idx >= ui->userNameComboBox->count())
        idx = 0 ;
    ui->userNameComboBox->setCurrentIndex(idx);
}

void ScreenshotWidget::on_nextPageButton_clicked() {
    if (m_currentPage < m_totalPages)
        setCurrentPage(m_currentPage+1);
}

void ScreenshotWidget::on_previousPageButton_clicked() {
    if (m_currentPage > 1)
        setCurrentPage(m_currentPage-1);
}
