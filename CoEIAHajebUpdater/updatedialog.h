#ifndef UPDATEDIALOG_H
#define UPDATEDIALOG_H

#include <QDialog>
#include <QMap>

namespace Ui {
class UpdateDialog;
}

class UpdateDialog : public QDialog
{
    Q_OBJECT
public:
    UpdateDialog(QStringList list,QWidget* parent=0);
    ~UpdateDialog();
private:
    void startDownload();
    void startHajeb();
    bool remove(QString fileName);
    bool rename(QString fileName);
    void download(QString fileName);
    bool isExist(QString fileName);
    void updateVersionFile();
    Ui::UpdateDialog* ui;
    QStringList updateList;
    QMap<QString,QString> exeName;
};

#endif // UPDATEDIALOG_H
