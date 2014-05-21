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
#ifndef ADDPROGRAMDIALOG_H
#define ADDPROGRAMDIALOG_H

#include <QDialog>
#include "registryentry.h"


namespace Ui {
class AddProgramDialog;
}

namespace ProgramLocker {
class ProgramModel;
}

class QProgressDialog;

class AddProgramDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddProgramDialog(QWidget *parent = 0);
    void build();
    QString processName() const;
    QStringList programList() const;
signals:
    void programListChoosed();
    void processChoosed();
public slots:
    void on_addButton_clicked();
    void on_browseButton_clicked();
    void updateProgressDialog(int i);
    void updateProgressDialog(QString fileName);
private:
    void createModel();
    void initView();
    Ui::AddProgramDialog* ui;
    ProgramLocker::ProgramModel* m_model;
    QProgressDialog* dlg;
    QString findLabel;
    QList<ProgramLocker::RegistryEntry> list;
};

#endif // ADDPROGRAMDIALOG_H
