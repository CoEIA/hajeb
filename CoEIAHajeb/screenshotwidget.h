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
#ifndef SCREENSHOTWIDGET_H
#define SCREENSHOTWIDGET_H

#include <QWidget>
#include <QFileInfoList>

namespace Ui {
    class ScreenshotWidget;
}

class QSqlTableModel;

class ScreenshotWidget : public QWidget {
    Q_OBJECT
public:
    ScreenshotWidget(QWidget* parent=0);
private slots:
    void on_nextPageButton_clicked();
    void on_previousPageButton_clicked();
    void on_updateScreenshotButton_clicked();
    void updateScreenshot();
    void on_clearScreenshotButton_clicked();
    void on_saveButton_clicked();
    void readUserScreenshot(int index);
    void readUserScreenshotTime(int index);
    void updateUsersModel();
    void updateImageListView(int pageNo);
signals:
    void pageChanged(int);
private:
    void createModel();
    void initListWidget();
    QFileInfoList getPageImage(int pageNo);
    void setCurrentPage(int page);
    Ui::ScreenshotWidget* ui;
    QSqlTableModel* userNameComboBoxModel;
    QString screenshotTitle;
    QFileInfoList imageList;
    int m_totalPages;
    const int MAX_IMG;
    int m_currentPage;
};

#endif // SCREENSHOTWIDGET_H
