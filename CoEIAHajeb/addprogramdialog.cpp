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
#include <QStandardItemModel>
#include <QProgressDialog>
#include <QFileInfo>
#include <QFileDialog>
#include <QDebug>

#include "addprogramdialog.h"
#include "ui_addprogramdialog.h"
#include "registryentry.h"
#include "registryreader.h"
#include "../Global/global.h"
#include "programmodel.h"

using namespace ProgramLocker;

AddProgramDialog::AddProgramDialog(QWidget *parent) :
    QDialog(parent),ui(new Ui::AddProgramDialog)
{
    ui->setupUi(this);
    ui->toolBox->setCurrentIndex(0);

    createModel();
    initView();
}

void AddProgramDialog::build() {
    dlg = new QProgressDialog(this);
    dlg->setWindowModality(Qt::WindowModal);

    findLabel = "البحث عن البرامج ... \n";
    dlg->setLabelText(findLabel);

    dlg->setMaximum(list.count());
    dlg->setMinimum(0);
    dlg->setValue(1);

    m_model->buildItem(list);

    dlg->setValue(list.count());
}

void AddProgramDialog::createModel() {
    // Read Program List from registry
    list.clear();
    RegistryReader reader;
    list << reader.read(HKLM_UNINSTALL_32);
    list << reader.read(HKCU_UNINSTALL_32);

    m_model = new ProgramModel(this);
    connect(m_model,SIGNAL(itemReaded(int)),SLOT(updateProgressDialog(int)));
    connect(m_model,SIGNAL(exeFounded(QString)),SLOT(updateProgressDialog(QString)));
}

void AddProgramDialog::initView() {
    ui->treeView->setModel(m_model);
}

void AddProgramDialog::on_addButton_clicked() {
    if (ui->toolBox->currentIndex() == 0 ) { // Program List
        qDebug() << "program list choice";
        emit programListChoosed();
    } else {
        qDebug() << "process name choice";
        emit processChoosed();
    }

    ui->lineEdit->clear();
    m_model->clearCacheList();
    this->accept();
}

void AddProgramDialog::on_browseButton_clicked() {
    QString programName = QFileDialog::getOpenFileName(this,"",QDir::root().path());
    QFileInfo fileInfo(programName);
    QString exeName = fileInfo.fileName();
    if (exeName.isEmpty())
        return ;
    ui->lineEdit->setText(exeName);
}


QString AddProgramDialog::processName() const {
    return ui->lineEdit->text();
}

QStringList AddProgramDialog::programList() const {
    return m_model->programList();
}

void AddProgramDialog::updateProgressDialog(int i) {
    dlg->setValue(i);
    QApplication::processEvents();
}

void AddProgramDialog::updateProgressDialog(QString fileName) {
    dlg->setLabelText(findLabel + fileName);
    QApplication::processEvents();
}

