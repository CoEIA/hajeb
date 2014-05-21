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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

//class HeaderWidget;
class QLabel;
class QStackedWidget;
class QSignalMapper;
class SlidingStackedWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum MenuButton {
        Home,
        UserSettings,
        ServiceLog,
        ScreenshotMonitor,
        SurfTime,
        WebsiteLocker,
        ProgramLocker
    };

    MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);
private slots:
    void setCurrentWindow(int id);
    void readAvailableDays();
signals:
    void updateCurrentDateTime();
    void updateLogs();
    void updateScreenshots();
    void updateStatistics();
private:
    void createDockWidget();
    void createStatusBar();
    SlidingStackedWidget* stackedWidget;
    //HeaderWidget* headerWidget;
    QWidget* headerWidget;
    QWidget* rightPanelWidget;
    QLabel* currentPageLabel;
    QLabel* versionLabel;
    QSignalMapper* signalMapper;
};

#endif // MAINWINDOW_H
