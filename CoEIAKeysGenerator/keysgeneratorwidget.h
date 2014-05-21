#ifndef KEYSGENERATORWIDGET_H
#define KEYSGENERATORWIDGET_H

#include <QWidget>
#include <QtSql/QtSql>

namespace Ui {
    class KeysGeneratorWidget;
}

class KeysGeneratorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KeysGeneratorWidget(QWidget *parent = 0);
    ~KeysGeneratorWidget();
public slots:
    void on_saveKeyButton_clicked();
    void on_removeKeyButton_clicked();
    void on_generate1000KeysButton_clicked();
    void on_generateButton_clicked();
private:
    QString getTime() ;
    QString getNextId();
    QString getRandomChars (int length);
    bool existKey (QString key);
    Ui::KeysGeneratorWidget *ui;
    QSqlTableModel *model ;
};

#endif // KEYSGENERATORWIDGET_H
