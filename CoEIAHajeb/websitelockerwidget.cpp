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
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QDateTime>
#include <QMessageBox>
#include <QInputDialog>

#include "websitelockerwidget.h"
#include "utility.h"
#include "ui_websitelockerwidget.h"
#include "useraccounts.h"
#include "../Global/global.h"

#include "treemodel.h"
#include "item.h"
#include "listimporter.h"
#include "listupdater.h"


WebsiteLockerWidget::WebsiteLockerWidget(QWidget *parent):
        QWidget(parent),ui(new Ui::WebsiteLockerWidget)
{
    ui->setupUi(this);
    blockSiteTitle = "قائمة المواقع للمستخدم ";
    ui->blockSiteGroupBox->setTitle(blockSiteTitle);

    ui->blackListRadioButton->setObjectName("blackListButton");
    ui->whiteListRadioButton->setObjectName("whiteListButton");

    connect(ui->blackListRadioButton,SIGNAL(clicked(bool)),SLOT(changeList(bool)));
    connect(ui->whiteListRadioButton,SIGNAL(clicked(bool)),SLOT(changeList(bool)));

    createModel();
    initView();
}

void WebsiteLockerWidget::createModel() {
    // Load model to userNameComboBox
    userNameComboBoxModel = new QSqlTableModel(this,QSqlDatabase::database(LSPDB_NAME));
    userNameComboBoxModel->setTable("UsersTable");
    userNameComboBoxModel->select();
    ui->userNameComboBox->setModel(userNameComboBoxModel);
    ui->userNameComboBox->setModelColumn(1);
    connect(ui->userNameComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(readUserData(int)));

    int index = ui->userNameComboBox->findText(getCurrentUserName());
    if (index == -1) {
        qDebug() << "ERROR -1 user index";
        index = 0 ;
    }

    model = new TreeModel(DB::ListTable::BLACK_LIST);
    ui->userNameComboBox->setCurrentIndex(index);

    readUserData(index);
}

void WebsiteLockerWidget::initView() {
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->treeView->setModel(model);
}

void WebsiteLockerWidget::on_addWebsiteButton_clicked() {
    if (ui->treeView->currentIndex() == QModelIndex()) {
        QMessageBox::warning(this,"لا يمكن إضافة موقع","فضلا قم بتحديد أحد التصنيفات لكي يتم اضافة الموقع بداخله");
        return ;
    }

    bool ok;
    QString url = QInputDialog::getText(this,"عنوان الموقع المراد حظره","ادخل عنوان الموقع . مثال: www.example.com",QLineEdit::Normal,QString(),&ok);
    if (!ok)
        return ;

    if (url.isEmpty()) {
        QMessageBox::warning(this,"لا يمكن حفظ السجل","قم بتحديد الموقع المراد حظره");
        return ;
    }


    QModelIndex index ;
    if (ui->treeView->currentIndex().parent().isValid())
        index = ui->treeView->currentIndex().parent();
    else
        index = ui->treeView->currentIndex();

    model->addWebsite(url,model->itemFromIndex(index));

//    // Add IP along with the site name.
//    QString ip = QString::fromStdString(Utility::IpFromHost(url.toStdString()));

//    save(url);
//    save(ip);

//    QString name = ui->userNameComboBox->currentText();
//    QMessageBox::information(this,"تم الحفظ بنجاح",QString("تم إضافة الموقع:\n %1 \nالى قائمة المواقع المحظورة للمستخدم %2").arg(url).arg(name));
}

void WebsiteLockerWidget::save(QString url) {
//    if (url.isEmpty() || url.isNull())
//        return;

//    int id = ui->userNameComboBox->model()->index(ui->userNameComboBox->currentIndex(),0).data().toInt();
//    QString host = url.remove("http://");
//    host = host.remove("www.");

//    QSqlQuery query(QSqlDatabase::database(LSPDB_NAME));
//    query.prepare("INSERT INTO LockedWebsitesTable(userId,url,host,lockDate) VALUES (?,?,?,?);");
//    query.addBindValue(id);
//    query.addBindValue(url);
//    query.addBindValue(host);
//    query.addBindValue(QDate::currentDate());
//    query.exec();
//    model->select();
}

//void WebsiteLockerWidget::on_deleteWebsiteButton_clicked() {


////    if (ui->tableView->currentIndex() == QModelIndex()) {
////        QMessageBox::warning(this,"لا يمكن الحذف","قم بتحديد موقع من القائمة لكي يتم حذفه");
////        return ;
////    }

////    int id = ui->userNameComboBox->model()->index(ui->userNameComboBox->currentIndex(),0).data().toInt();
////    int no = model->index(ui->tableView->currentIndex().row(),0).data().toInt();
////    QString host = model->index(ui->tableView->currentIndex().row(),3).data().toString();

////    QSqlQuery query(QSqlDatabase::database(LSPDB_NAME));
////    query.prepare("DELETE FROM LockedWebsitesTable WHERE userId = ? AND no = ?");
////    query.addBindValue(id);
////    query.addBindValue(no);
////    query.exec();

////    QString hostIp = QString::fromStdString(Utility::IpFromHost(host.toStdString()));

////    query.prepare("DELETE FROM LockedWebsitesTable WHERE userId = ? AND host = ?");
////    query.addBindValue(id);
////    query.addBindValue(hostIp);
////    query.exec();

////    model->setFilter(QString("LockedWebsitesTable.userId = %1").arg(id));
//}

int WebsiteLockerWidget::userId() {
    return ui->userNameComboBox->model()->index(ui->userNameComboBox->currentIndex(),0).data().toInt();
}

void WebsiteLockerWidget::readUserData(int index) {
    if (!saveChanges())
        return ;

    // update groupbox title
    QString title = blockSiteTitle + ui->userNameComboBox->currentText() + " ";
    qDebug() << "user name: " << ui->userNameComboBox->currentText();
    ui->blockSiteGroupBox->setTitle(title);

    int id = ui->userNameComboBox->model()->index(index,0).data().toInt();

    // read and set user list type.
    readUserListType(id);

    model->import(id);
}

ListType WebsiteLockerWidget::readUserListType(int userId) {
    ListType type = (ListType)model->readUserListType(userId);
    if (type == BLACK_LIST)
        ui->blackListRadioButton->setChecked(true);
    else
        ui->whiteListRadioButton->setChecked(true);
}

void WebsiteLockerWidget::writeUserListType(ListType type) {
    model->writeUserListType((DB::ListTable::ListType)type);
}

void WebsiteLockerWidget::updateUsersModel() {
    int idx = ui->userNameComboBox->currentIndex();
    userNameComboBoxModel->select();
    if (idx < 0 || idx >= ui->userNameComboBox->count())
        idx = 0 ;
    ui->userNameComboBox->setCurrentIndex(idx);
}

void WebsiteLockerWidget::on_addCategoryButton_clicked()
{
    bool ok;
    QString catName = QInputDialog::getText(this,"إنشاء تصنيف جديد","أدخل اسم التصنيف",QLineEdit::Normal,QString(),&ok);
    if (!ok)
        return ;

    if (catName.isEmpty()) {
        QMessageBox::warning(this,"لا يمكن حفظ السجل","قم بتحديد اسم للتصنيف");
        return ;
    }

    model->addCategory(catName);
}

void WebsiteLockerWidget::on_saveButton_clicked()
{
    model->save();

    // Save current list in the lsp block table.
    model->exportToLSPDb();

    // Also save current settings (white list, black list).
    if (ui->blackListRadioButton->isChecked())
        model->writeUserListType(DB::ListTable::BLACK_LIST);
    else
        model->writeUserListType(DB::ListTable::WHITE_LIST);
}

void WebsiteLockerWidget::on_deleteButton_clicked()
{
    if (ui->treeView->currentIndex() == QModelIndex()) {
        QMessageBox::warning(this,"لا يمكن الحذف","فضلا قم بتحديد الموقع أو التصنيف المراد حذفه");
        qDebug() << "No current index";
        return ;
    }

    model->removeItem(model->itemFromIndex(ui->treeView->currentIndex()));
}

void WebsiteLockerWidget::on_resetButton_clicked()
{
    qDebug() << "reset clicked";
    ListImporter listImporter(model->database(),(ListImporter::ListType)model->listType());
    if (listImporter.import() == NULL) {
        QMessageBox::information(this,"قم بتحديث القائمة","فضلا قم بتحديث القائمة أولا حتى تتمكن من استخدام الخصائص الأخرى");
        return;
    }
    listImporter.printList();
    listImporter.resetDB();

    // update the model
    int id = ui->userNameComboBox->model()->index(ui->userNameComboBox->currentIndex(),0).data().toInt();
    model->import(id);
}

void WebsiteLockerWidget::on_mergeButton_clicked()
{
    ListImporter listImporter(model->database(),(ListImporter::ListType)model->listType());
    if (listImporter.import() == NULL) {
        QMessageBox::information(this,"قم بتحديث القائمة","فضلا قم بتحديث القائمة أولا حتى تتمكن من استخدام الخصائص الأخرى");
        return;
    }
    listImporter.printList();
    listImporter.updateDB();

    // update the model
    int id = ui->userNameComboBox->model()->index(ui->userNameComboBox->currentIndex(),0).data().toInt();
    model->import(id);
}

void WebsiteLockerWidget::on_updateButton_clicked()
{
    ListUpdater* updater = new ListUpdater((ListType)model->listType(),this);
    connect(updater,SIGNAL(finished(UpdateResult)),SLOT(updateFiles(UpdateResult)));
    connect(updater,SIGNAL(finished(UpdateResult)),SLOT(finishUpdate(UpdateResult)));

    updater->update();

    ui->updateButton->setText("جاري التحديث");
    ui->updateButton->setEnabled(false);
}

void WebsiteLockerWidget::changeList(bool isChecked) {
    if (!saveChanges()) {
        if (ui->blackListRadioButton->isChecked())
            ui->whiteListRadioButton->setChecked(true);
        else
            ui->blackListRadioButton->setChecked(true);
        return ;
    }

    qDebug() << "value changed";

    if (sender()->objectName() == "blackListButton") {
        if (isChecked) {
            qDebug() << "Import Black List ..";
            model->setListType(DB::ListTable::BLACK_LIST);
            model->import(userId());
        }
    } else {
        if (isChecked) {
            qDebug() << "Import White List ..";
            model->setListType(DB::ListTable::WHITE_LIST);
            model->import(userId());
        }
    }
}

void WebsiteLockerWidget::updateFiles(UpdateResult result) {
    if (result == NO_UPDATE)
        return ;

    ListImporter listImporter(model->database(),(ListImporter::ListType)model->listType());
    listImporter.import();
    listImporter.printList();
    listImporter.updateDB();

    // update the model
    model->import(userId());
}


void WebsiteLockerWidget::on_expandButton_clicked()
{
    ui->treeView->expandAll();
}

void WebsiteLockerWidget::on_colapseButton_clicked()
{
    ui->treeView->collapseAll();
}

bool WebsiteLockerWidget::saveChanges() {
    if (model->hasChanged())
        return saveDialog();
    else
        return true;
}

bool WebsiteLockerWidget::saveDialog() {
    int ret = QMessageBox::information(this,"تم التعديل على القائمة","هل تريد حفظ التغييرات؟",QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel,QMessageBox::Save);
    if (ret == QMessageBox::Cancel)
        return false;

    if (ret == QMessageBox::Save)
        this->on_saveButton_clicked();
    else if (ret == QMessageBox::Discard)
        model->clearActions();

    return true;    // on Save and Discard
}

void WebsiteLockerWidget::finishUpdate(UpdateResult result) {
    if (result == NO_UPDATE) {
        QMessageBox::information(this,"لا توجد تحديثات","لا توجد تحديثات جديدة.");
    }

    ui->updateButton->setText("تحديث القائمة  ");
    ui->updateButton->setEnabled(true);
}
