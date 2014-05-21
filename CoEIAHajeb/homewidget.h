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
#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>
#include <QDialog>

namespace Ui {
    class HomeWidget;
}

class UserAccountsWidget;
class QLabel;

class HomeWidget : public QWidget
{
    Q_OBJECT
public:
    HomeWidget(QWidget* parent=0);
private slots:
    void enableTaskManger(bool isChecked);
    void on_userButton_clicked();
    void on_registerKeyButton_clicked();
    void hajebservicesButtonToggled(bool);
    void on_updateButton_clicked();
    void readApplicationStatus();
    void checkForUpdate();
signals:
    void updateModel();
    void registerd();
private:
    void readTaskMangerStatus();
    void readHajebServicesStatus();
    void updateHajebVersionLabels();
    void verifyKey(QString serialNumber);
    Ui::HomeWidget* ui;
    UserAccountsWidget* userAccountsWidget;
};

#endif // HOMEWIDGET_H
