#ifndef ADDPROGRAMDIALOG_H
#define ADDPROGRAMDIALOG_H

#include <QDialog>
#include "registryentry.h"


namespace Ui {
class AddProgramDialog;
}

namespace ProgramLocker {
class ProgramModel;
}

class QProgressDialog;

class AddProgramDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddProgramDialog(QWidget *parent = 0);
    void build();
    QString processName() const;
    QStringList programList() const;
signals:
    void programListChoosed();
    void processChoosed();
public slots:
    void on_addButton_clicked();
    void on_browseButton_clicked();
    void updateProgressDialog(int i);
    void updateProgressDialog(QString fileName);
private:
    void createModel();
    void initView();
    Ui::AddProgramDialog* ui;
    ProgramLocker::ProgramModel* m_model;
    QProgressDialog* dlg;
    QString findLabel;
    QList<ProgramLocker::RegistryEntry> list;
};

#endif // ADDPROGRAMDIALOG_H
