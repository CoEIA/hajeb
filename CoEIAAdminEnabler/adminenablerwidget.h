#ifndef ADMINENABLERWIDGET_H
#define ADMINENABLERWIDGET_H

#include <QWidget>

namespace Ui {
    class AdminEnablerWidget;
}

class AdminEnablerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AdminEnablerWidget(QWidget *parent = 0);
    ~AdminEnablerWidget();
private slots:
    void on_enableAdminButton_clicked();
    void on_disableAdminButton_clicked();
    void on_exitButton_clicked();
private:
    Ui::AdminEnablerWidget *ui;
};

#endif // ADMINENABLERWIDGET_H
