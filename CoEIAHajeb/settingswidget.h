#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>

namespace Ui
{
    class SettingsWidget;
}

class QSqlTableModel;

class SettingsWidget : public QWidget
{
    Q_OBJECT
public:
    SettingsWidget(QWidget* parent=0);
private slots:
    void on_saveSettingsButton_clicked();
    void readUserSettings(int index);
    void on_updateStatisticsButton_clicked();
    void updateStatistics();
    void updateUsersModel();
private:
    void createModel();
    Ui::SettingsWidget* ui;
    QSqlTableModel* userNameComboBoxModel;
    QString settingsTitle;
    QString statisticsTitle;
};



#endif // SETTINGSWIDGET_H
