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
#ifndef BROWSETIMEWIDGET_H
#define BROWSETIMEWIDGET_H

#include <QWidget>

namespace Ui {
    class BrowseTimeWidget;
}

class QSqlTableModel;
class QToolButton;

class BrowseTimeWidget : public QWidget
{
    Q_OBJECT
public:
    enum BrowseTimeMethod {
        TimeTable,
        TimeCounter
    };

    BrowseTimeWidget(QWidget* parent=0);
private slots:
    void on_disableAllButton_clicked();
    void on_enableAllButton_clicked();
    void readSettings(int index);
    void on_saveButton_clicked();
    void updateUsersModel();
    void setPage();
private:
    void createModel();
    void updateTimeTable(QList<QPair<int,int> > timesTable,int id);
    void updateTimeCounter(QList<QPair<int,QTime> > timesCounter,int id);
    void saveTimeTable(int index);
    void saveTimeCounter(int index);
    BrowseTimeMethod browseTimeMethod(int index);
    void setBrowseTimeMethod(int index,BrowseTimeWidget::BrowseTimeMethod method);
    void readUserTimeTableSettings(int index);
    void readUserTimeCounterSettings(int index);
    void hideFlagButton(bool ok);
    Ui::BrowseTimeWidget* ui;
    QSqlTableModel* userNameComboBoxModel;
    QString groupBoxTitle;
    QString counterGroupBoxTitle;

};

#endif // BROWSETIMEWIDGET_H
