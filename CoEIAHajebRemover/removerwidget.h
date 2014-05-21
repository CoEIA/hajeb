#ifndef REMOVERWIDGET_H
#define REMOVERWIDGET_H

#include <QWidget>

namespace Ui {
    class RemoverWidget;
}

class RemoverWidget : public QWidget
{
    Q_OBJECT
public:
    RemoverWidget(QWidget* parent=0);
    ~RemoverWidget();

private slots:
    void on_removeButton_clicked();
    void on_cancelButton_clicked();
private:
    Ui::RemoverWidget* ui;
};

#endif // REMOVERWIDGET_H
