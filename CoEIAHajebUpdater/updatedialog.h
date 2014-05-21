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
#ifndef UPDATEDIALOG_H
#define UPDATEDIALOG_H

#include <QDialog>
#include <QMap>

namespace Ui {
class UpdateDialog;
}

class UpdateDialog : public QDialog
{
    Q_OBJECT
public:
    UpdateDialog(QStringList list,QWidget* parent=0);
    ~UpdateDialog();
private:
    void startDownload();
    void startHajeb();
    bool remove(QString fileName);
    bool rename(QString fileName);
    void download(QString fileName);
    bool isExist(QString fileName);
    void updateVersionFile();
    Ui::UpdateDialog* ui;
    QStringList updateList;
    QMap<QString,QString> exeName;
};

#endif // UPDATEDIALOG_H
