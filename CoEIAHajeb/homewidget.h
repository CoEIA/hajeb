#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>
#include <QDialog>

namespace Ui {
    class HomeWidget;
}

class UserAccountsWidget;
class QLabel;

class HomeWidget : public QWidget
{
    Q_OBJECT
public:
    HomeWidget(QWidget* parent=0);
private slots:
    void enableTaskManger(bool isChecked);
    void on_userButton_clicked();
    void on_registerKeyButton_clicked();
    void hajebservicesButtonToggled(bool);
    void on_updateButton_clicked();
    void readApplicationStatus();
    void checkForUpdate();
signals:
    void updateModel();
    void registerd();
private:
    void readTaskMangerStatus();
    void readHajebServicesStatus();
    void updateHajebVersionLabels();
    void verifyKey(QString serialNumber);
    Ui::HomeWidget* ui;
    UserAccountsWidget* userAccountsWidget;
};

#endif // HOMEWIDGET_H
