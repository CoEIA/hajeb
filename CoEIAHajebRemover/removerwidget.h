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
#ifndef REMOVERWIDGET_H
#define REMOVERWIDGET_H

#include <QWidget>

namespace Ui {
    class RemoverWidget;
}

class RemoverWidget : public QWidget
{
    Q_OBJECT
public:
    RemoverWidget(QWidget* parent=0);
    ~RemoverWidget();

private slots:
    void on_removeButton_clicked();
    void on_cancelButton_clicked();
private:
    Ui::RemoverWidget* ui;
};

#endif // REMOVERWIDGET_H
