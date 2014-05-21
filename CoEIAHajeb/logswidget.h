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
#ifndef LOGSWIDGET_H
#define LOGSWIDGET_H

#include <QWidget>

class QSqlTableModel;

namespace Ui {
    class LogsWidget;
}
class LogsWidget : public QWidget
{
    Q_OBJECT
public:
    LogsWidget(QWidget* parent=0);
private slots:
    void on_updateLogButton_clicked();
    void updateLog();
    void on_clearLogButton_clicked();
    void readUserLog(int index);
    void updateUsersModel();
private:
    void createModel();
    void initView();
    Ui::LogsWidget* ui;
    QSqlTableModel* programLogModel;
    QSqlTableModel* blockedWebsiteLogModel;
    QSqlTableModel* browseWebLogModel;
    QSqlTableModel* keyloggerLogModel;
    QSqlTableModel* userNameComboBoxModel;
    QString logsTitle;
};

#endif // LOGSWIDGET_H
