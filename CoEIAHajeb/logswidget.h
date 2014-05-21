#ifndef LOGSWIDGET_H
#define LOGSWIDGET_H

#include <QWidget>

class QSqlTableModel;

namespace Ui {
    class LogsWidget;
}
class LogsWidget : public QWidget
{
    Q_OBJECT
public:
    LogsWidget(QWidget* parent=0);
private slots:
    void on_updateLogButton_clicked();
    void updateLog();
    void on_clearLogButton_clicked();
    void readUserLog(int index);
    void updateUsersModel();
private:
    void createModel();
    void initView();
    Ui::LogsWidget* ui;
    QSqlTableModel* programLogModel;
    QSqlTableModel* blockedWebsiteLogModel;
    QSqlTableModel* browseWebLogModel;
    QSqlTableModel* keyloggerLogModel;
    QSqlTableModel* userNameComboBoxModel;
    QString logsTitle;
};

#endif // LOGSWIDGET_H
