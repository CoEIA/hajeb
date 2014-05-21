#ifndef BROWSETIMEWIDGET_H
#define BROWSETIMEWIDGET_H

#include <QWidget>

namespace Ui {
    class BrowseTimeWidget;
}

class QSqlTableModel;
class QToolButton;

class BrowseTimeWidget : public QWidget
{
    Q_OBJECT
public:
    enum BrowseTimeMethod {
        TimeTable,
        TimeCounter
    };

    BrowseTimeWidget(QWidget* parent=0);
private slots:
    void on_disableAllButton_clicked();
    void on_enableAllButton_clicked();
    void readSettings(int index);
    void on_saveButton_clicked();
    void updateUsersModel();
    void setPage();
private:
    void createModel();
    void updateTimeTable(QList<QPair<int,int> > timesTable,int id);
    void updateTimeCounter(QList<QPair<int,QTime> > timesCounter,int id);
    void saveTimeTable(int index);
    void saveTimeCounter(int index);
    BrowseTimeMethod browseTimeMethod(int index);
    void setBrowseTimeMethod(int index,BrowseTimeWidget::BrowseTimeMethod method);
    void readUserTimeTableSettings(int index);
    void readUserTimeCounterSettings(int index);
    void hideFlagButton(bool ok);
    Ui::BrowseTimeWidget* ui;
    QSqlTableModel* userNameComboBoxModel;
    QString groupBoxTitle;
    QString counterGroupBoxTitle;

};

#endif // BROWSETIMEWIDGET_H
