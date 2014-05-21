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
#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui/QWidget>
#include <QDate>
#include <QSystemTrayIcon>
class QMenu;
class QAction;
class QTimer;

namespace Ui {
    class Widget;
}
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
protected:
    void closeEvent(QCloseEvent *);
    void paintEvent(QPaintEvent *);
    void hideEvent(QHideEvent *event);
    void showEvent(QShowEvent *event);
private slots:
    void readUserTimeTable();
    void showMessage();
    void showClicked();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void increaseOpacityWidget();
    void decreaseOpacityWidget();
    void on_closeButton_clicked();
    void on_startButton_toggled(bool checked);
private:
    void createActions();
    void createMenus();
    QString getCurrentUserName(int userId);
    QString getCurrentUserName();
    int getCurrentUserId();
    int getCurrentUserId(QString userName);
    bool isEnabledForUser(int id);
    int readTodayTime(int userId);
    void moveWidgetToBottom();
    void createTryIcon();
    void updateClock();
    void showUserNotControllWidget();
    void showMonitorIsDisabled();
    void showTimeTableWidget();
    void showCounterWidget();
    void updateTimeTable(int todayTiming);
    void updateCounter(int hour,int min,int sec);
    int readNetCmd();
    void writeNetCmd(int i);
    int userMethod(int id);
    void timeTableMethod(int);
    void counterMethod(int userId);
    int currentDate();
    QTime readTodayRemaingingCounter(int id);
    QTime readTodayAllowedCounter(int id);
    QDate readCounterLastDate(int id);

    void writeTodayRemainingCounter(int id,QTime time);
    void writeCounterLastDate(int id);

    QSystemTrayIcon* trayIcon;
    QMenu* trayIconMenu;
    QAction* maximizeAct;
    QAction* minimizeAct;
    QAction* aboutAct;
    QTimer* m_timer;
    QTimer* m_showTimer;
    Ui::Widget* ui;
    int previousTime;
    int previousHour;
    int previousMin;
    int m_opcaity;
    bool m_settingsDisabled;

    bool isCounterStopped;
};

#endif // WIDGET_H
