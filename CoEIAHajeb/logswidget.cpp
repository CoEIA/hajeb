#include <QSqlQuery>
#include <QSqlTableModel>
#include <QMessageBox>
#include "logswidget.h"
#include "ui_logswidget.h"
#include "useraccounts.h"
#include "../Global/global.h"

LogsWidget::LogsWidget(QWidget *parent):
        QWidget(parent),ui(new Ui::LogsWidget)
{
    ui->setupUi(this);
    logsTitle = "سجل الخدمة للمستخدم ";
    ui->logsGroupBox->setTitle(logsTitle);

    ui->tabWidget->setCurrentIndex(0);
    createModel();
    initView();
    int index = ui->userNameComboBox->findText(getCurrentUserName());
    if (index == -1)
        index = 0 ;
    ui->userNameComboBox->setCurrentIndex(index);
    readUserLog(index);
}

void LogsWidget::createModel() {
    // Blocked program log
    programLogModel = new QSqlTableModel(this,QSqlDatabase::database(PCSDB_NAME));
    programLogModel->setTable("LogsTable");
    programLogModel->setHeaderData(2,Qt::Horizontal,"الحدث");
    programLogModel->setHeaderData(3,Qt::Horizontal,"التاريخ");
    programLogModel->setHeaderData(4,Qt::Horizontal,"الوقت");
    programLogModel->select();

    // Blocked website browse log
    blockedWebsiteLogModel = new QSqlTableModel(this,QSqlDatabase::database(LSPDB_NAME));
    blockedWebsiteLogModel->setTable("LogsTable");
    blockedWebsiteLogModel->setHeaderData(2,Qt::Horizontal,"الحدث");
    blockedWebsiteLogModel->setHeaderData(3,Qt::Horizontal,"التاريخ");
    blockedWebsiteLogModel->setHeaderData(4,Qt::Horizontal,"الوقت");
    blockedWebsiteLogModel->select();

    // Internet browse log
    browseWebLogModel = new QSqlTableModel(this,QSqlDatabase::database(LSPDB_NAME));
    browseWebLogModel->setTable("BrowseLogsTable");
    browseWebLogModel->setHeaderData(2,Qt::Horizontal,"عنوان النطاق");
    browseWebLogModel->setHeaderData(3,Qt::Horizontal,"عنوان الموقع");
    browseWebLogModel->setHeaderData(4,Qt::Horizontal,"التاريخ");
    browseWebLogModel->setHeaderData(5,Qt::Horizontal,"الوقت");
    browseWebLogModel->select();

    // Keylogger log
    keyloggerLogModel = new QSqlTableModel(this,QSqlDatabase::database(PCSDB_NAME));
    keyloggerLogModel->setTable("KeylogsTable");
    keyloggerLogModel->setHeaderData(2,Qt::Horizontal,"اسم البرنامج");
    keyloggerLogModel->setHeaderData(3,Qt::Horizontal,"السجل");
    keyloggerLogModel->setHeaderData(4,Qt::Horizontal,"التاريخ");
    keyloggerLogModel->setHeaderData(5,Qt::Horizontal,"الوقت");
    keyloggerLogModel->select();

    // Load model to userNameComboBox
    userNameComboBoxModel = new QSqlTableModel(this,QSqlDatabase::database(PCSDB_NAME));
    userNameComboBoxModel->setTable("UsersTable");
    userNameComboBoxModel->select();
    ui->userNameComboBox->setModel(userNameComboBoxModel);
    ui->userNameComboBox->setModelColumn(1);
    connect(ui->userNameComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(readUserLog(int)));
}

void LogsWidget::initView() {
    ui->programLogTableView->setModel(programLogModel);
    ui->programLogTableView->setObjectName("programLogTableView");
    //ui->programLogTableView->setAlternatingRowColors(true);
    ui->programLogTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->programLogTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->programLogTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->programLogTableView->setColumnHidden(0,true);
    ui->programLogTableView->setColumnHidden(1,true);
    ui->programLogTableView->setColumnHidden(5,true);
    QHeaderView* headerView = ui->programLogTableView->horizontalHeader();
    headerView->setResizeMode(2,QHeaderView::ResizeToContents);
    headerView->setStretchLastSection(true);

    ui->websiteLogTableView->setModel(blockedWebsiteLogModel);
    ui->websiteLogTableView->setObjectName("websiteLogTableView");
    //ui->websiteLogTableView->setAlternatingRowColors(true);
    ui->websiteLogTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->websiteLogTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->websiteLogTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->websiteLogTableView->setColumnHidden(0,true);
    ui->websiteLogTableView->setColumnHidden(1,true);
    ui->websiteLogTableView->setColumnHidden(5,true);
    headerView = ui->websiteLogTableView->horizontalHeader();
    headerView->setResizeMode(2,QHeaderView::ResizeToContents);
    headerView->setStretchLastSection(true);

    ui->browseWebLogTableView->setModel(browseWebLogModel);
    ui->browseWebLogTableView->setObjectName("browseWebLogTableView");
    //ui->browseWebLogTableView->setAlternatingRowColors(true);
    ui->browseWebLogTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->browseWebLogTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->browseWebLogTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->browseWebLogTableView->setColumnHidden(0,true);
    ui->browseWebLogTableView->setColumnHidden(1,true);
    headerView = ui->browseWebLogTableView->horizontalHeader();
    headerView->setResizeMode(2,QHeaderView::ResizeToContents);
    headerView->setStretchLastSection(true);

    ui->keyloggerLogTableView->setModel(keyloggerLogModel);
    ui->keyloggerLogTableView->setObjectName("keyloggerLogTableView");
    //ui->keyloggerLogTableView->setAlternatingRowColors(true);
    ui->keyloggerLogTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->keyloggerLogTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->keyloggerLogTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->keyloggerLogTableView->setColumnHidden(0,true);
    ui->keyloggerLogTableView->setColumnHidden(1,true);
    headerView = ui->keyloggerLogTableView->horizontalHeader();
    headerView->setResizeMode(3,QHeaderView::ResizeToContents);
    headerView->setStretchLastSection(true);
}

void LogsWidget::on_updateLogButton_clicked() {
    updateLog();
    QMessageBox::information(this,"تم تحديث السجل",QString("تم تحديث السجل للمستخدم %1.").arg(ui->userNameComboBox->currentText()));
}

void LogsWidget::updateLog() {
    int id = ui->userNameComboBox->model()->index(ui->userNameComboBox->currentIndex(),0).data().toInt();
    programLogModel->setFilter(QString("LogsTable.userId = %1").arg(id));
    blockedWebsiteLogModel->setFilter(QString("LogsTable.userId = %1").arg(id));
    browseWebLogModel->setFilter(QString("BrowseLogsTable.userId = %1").arg(id));
    keyloggerLogModel->setFilter(QString("KeylogsTable.userId = %1").arg(id));
}

void LogsWidget::on_clearLogButton_clicked() {
    int id = ui->userNameComboBox->model()->index(ui->userNameComboBox->currentIndex(),0).data().toInt();
    if (ui->tabWidget->currentIndex() == 0) {
        QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
        query.prepare("DELETE FROM LogsTable WHERE userId = ?");
        query.addBindValue(id);
        query.exec();
        programLogModel->setFilter(QString("LogsTable.userId = %1").arg(id));
    } else if (ui->tabWidget->currentIndex() == 1) {
        QSqlQuery query(QSqlDatabase::database(LSPDB_NAME));
        query.prepare("DELETE FROM LogsTable WHERE userId = ?");
        query.addBindValue(id);
        query.exec();
        blockedWebsiteLogModel->setFilter(QString("LogsTable.userId = %1").arg(id));
    } else if (ui->tabWidget->currentIndex() == 2) {
        QSqlQuery query(QSqlDatabase::database(LSPDB_NAME));
        query.prepare("DELETE FROM BrowseLogsTable WHERE userId = ?");
        query.addBindValue(id);
        query.exec();
        browseWebLogModel->setFilter(QString("BrowseLogsTable.userId = %1").arg(id));
    } else {
        QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
        query.prepare("DELETE FROM KeylogsTable WHERE userId = ?");
        query.addBindValue(id);
        query.exec();
        keyloggerLogModel->setFilter(QString("KeylogsTable.userId = %1").arg(id));
    }
    QMessageBox::information(this,"تم حذف السجل",QString("تم حذف السجل للمستخدم %1.").arg(ui->userNameComboBox->currentText()));
}

void LogsWidget::readUserLog(int index) {
    // update groupbox title
    QString title = logsTitle + ui->userNameComboBox->currentText() + " ";
    ui->logsGroupBox->setTitle(title);

    int id = ui->userNameComboBox->model()->index(index,0).data().toInt();
    programLogModel->setFilter(QString("LogsTable.userId = %1").arg(id));
    blockedWebsiteLogModel->setFilter(QString("LogsTable.userId = %1").arg(id));
    browseWebLogModel->setFilter(QString("BrowseLogsTable.userId = %1").arg(id));
    keyloggerLogModel->setFilter(QString("KeylogsTable.userId = %1").arg(id));
}

void LogsWidget::updateUsersModel() {
    int idx = ui->userNameComboBox->currentIndex();
    userNameComboBoxModel->select();
    if (idx < 0 || idx >= ui->userNameComboBox->count())
        idx = 0 ;
    ui->userNameComboBox->setCurrentIndex(idx);
}
