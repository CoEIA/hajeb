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
#include "keysgeneratorwidget.h"
#include "ui_keysgeneratorwidget.h"

#include "../Global/keygenerator.h"
#include "../Global/keyvalidator.h"

#include <QMessageBox>

KeysGeneratorWidget::KeysGeneratorWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::KeysGeneratorWidget)
{
    ui->setupUi(this);
    model = new QSqlTableModel;
    model->setTable("GeneratedTable");
    model->select();

    ui->tableView->setModel(model);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->resizeColumnToContents(1);
    ui->tableView->resizeColumnToContents(0);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

KeysGeneratorWidget::~KeysGeneratorWidget()
{
    delete ui;
}

void KeysGeneratorWidget:: on_generateButton_clicked() {

    QString time = getTime();
    QString id = getNextId();
    QString constantNumber = "853023";

    QString key = KeyGenerator::generateKey(time, id, constantNumber);

    if ( !key.endsWith("+") && KeyValidator::validate(key) ) {
        ui->serialLineEdit->setText(key);
    }
}

void KeysGeneratorWidget::on_generate1000KeysButton_clicked() {
    int count = 0;
    for (int i=0; i<100 ; i++ ){
        QString time = getTime();
        QString id = getNextId();
        QString constantNumber = "853023";

        QString key = KeyGenerator::generateKey(time, id, constantNumber);

        if ( KeyValidator::validate(key) ) {
            ui->serialLineEdit->setText(key);

            if ( ! existKey (key) && ! key.endsWith("+") ) {
                QSqlQuery query;
                QString time = QDate::currentDate().toString();
                if ( query.exec("insert into GeneratedTable(serial, generateTime) values('" + key + "' , '" + time +"')") ) {
                    qDebug() << "key: " << key;
                    count++;
                }
            }
        }
    }

    qDebug() << "Number of Key Saved: " << count;
}

void KeysGeneratorWidget::on_saveKeyButton_clicked() {
    QString key = ui->serialLineEdit->text();

    if ( KeyValidator::validate(key) ) {
        if ( ! existKey (key) && ! key.endsWith("+") ) {
            QSqlQuery query;
            QString time = QDate::currentDate().toString();
            if ( query.exec("insert into GeneratedTable(serial, generateTime) values('" + key + "' , '" + time +"')") )
                QMessageBox::information(this,"inserting Ok","inserting Ok");
            else
                QMessageBox::warning(this,"cannot inserting key","cannot inserting key");
        }
        else {
            QMessageBox::warning(this,"Key existing in DB","Key existing in DB");
        }
    }
    else {
        QMessageBox::warning(this,"is not valid key", key + " is not valid key");
    }

    model->select();
}

void KeysGeneratorWidget::on_removeKeyButton_clicked() {
    int no = model->index(ui->tableView->currentIndex().row(),0).data().toInt();

    QSqlQuery query;
    query.prepare("DELETE FROM GeneratedTable WHERE id = ?");
    query.addBindValue(no);

    query.exec();

    model->select();
}


bool KeysGeneratorWidget:: existKey (QString key) {
   QSqlQuery query;

   query.prepare("SELECT * FROM GeneratedTable WHERE serial = ?");
   query.addBindValue(key);
   query.exec();

   while (query.next()) {
       return true;
   }

   return false;
}

QString KeysGeneratorWidget:: getTime () {
    QTime time = QTime::currentTime();
    QString result = QString::number(time.msec());

    return result;
}

QString KeysGeneratorWidget:: getNextId () {
    QString id = getRandomChars(5);
    return id;
}

QString KeysGeneratorWidget:: getRandomChars (int length) {
    int difference = 'Z'-'A';
    QString result = "";

    for (int i=0; i<length; i++) {
        char c = 'A' + ( rand() % difference );
        result += c ;
    }

    return result;
}
