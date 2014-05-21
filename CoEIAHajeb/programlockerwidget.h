#ifndef PROGRAMLOCKERWIDGET_H
#define PROGRAMLOCKERWIDGET_H

#include <QWidget>
#include <QDialog>

namespace Ui {
    class ProgramLockerWidget;
}

class QSqlTableModel;
class QLineEdit;
class AddProgramDialog;

class ProgramLockerWidget : public QWidget
{
    Q_OBJECT
public:
    ProgramLockerWidget(QWidget* parent=0);
private slots:
    void readUserData(int);
    void on_addButton_clicked();
    void on_deleteButton_clicked();
    void updateUsersModel();
    void saveProcess();
    void saveProgramList();
private:
    void createModel();
    void initView();
    void save(QString processName);
    Ui::ProgramLockerWidget* ui;
    QSqlTableModel* model;
    QSqlTableModel* userNameComboBoxModel;
    QString programTitle;
    AddProgramDialog* addProgramDialog;
};


#endif // PROGRAMLOCKERWIDGET_H
