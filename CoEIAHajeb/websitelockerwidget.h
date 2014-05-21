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
#ifndef WEBSITELOCKERWIDGET_H
#define WEBSITELOCKERWIDGET_H

#include <QWidget>


namespace Ui {
    class WebsiteLockerWidget;
}

class QSqlTableModel;

#include "websitelocker.h"
using namespace WebsiteLocker;

#include "xmlconstants.h"

class WebsiteLockerWidget : public QWidget
{
    Q_OBJECT
public:
    WebsiteLockerWidget(QWidget* parent=0);
private slots:
    void readUserData(int);
//    void on_deleteWebsiteButton_clicked();
    void updateUsersModel();
    void on_addWebsiteButton_clicked();
    void on_addCategoryButton_clicked();
    void on_saveButton_clicked();
    void on_deleteButton_clicked();
    void on_resetButton_clicked();
    void on_mergeButton_clicked();
    void on_updateButton_clicked();
    void changeList(bool isChecked);
    void on_expandButton_clicked();
    void on_colapseButton_clicked();
    void finishUpdate(UpdateResult result);
    void updateFiles(UpdateResult result);
private:
    void createModel();
    void initView();
    void save(QString url);
    int userId();
    ListType readUserListType(int userId);
    void writeUserListType(ListType type);
    bool saveChanges();
    bool saveDialog();

    Ui::WebsiteLockerWidget* ui;
    WebsiteLocker::TreeModel* model;
    QSqlTableModel* userNameComboBoxModel;
    QString blockSiteTitle;
};

#endif // WEBSITELOCKERWIDGET_H
