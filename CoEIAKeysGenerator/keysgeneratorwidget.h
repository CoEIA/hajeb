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
#ifndef KEYSGENERATORWIDGET_H
#define KEYSGENERATORWIDGET_H

#include <QWidget>
#include <QtSql/QtSql>

namespace Ui {
    class KeysGeneratorWidget;
}

class KeysGeneratorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KeysGeneratorWidget(QWidget *parent = 0);
    ~KeysGeneratorWidget();
public slots:
    void on_saveKeyButton_clicked();
    void on_removeKeyButton_clicked();
    void on_generate1000KeysButton_clicked();
    void on_generateButton_clicked();
private:
    QString getTime() ;
    QString getNextId();
    QString getRandomChars (int length);
    bool existKey (QString key);
    Ui::KeysGeneratorWidget *ui;
    QSqlTableModel *model ;
};

#endif // KEYSGENERATORWIDGET_H
