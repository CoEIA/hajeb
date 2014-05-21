#ifndef WEBSITELOCKERWIDGET_H
#define WEBSITELOCKERWIDGET_H

#include <QWidget>


namespace Ui {
    class WebsiteLockerWidget;
}

class QSqlTableModel;

#include "websitelocker.h"
using namespace WebsiteLocker;

#include "xmlconstants.h"

class WebsiteLockerWidget : public QWidget
{
    Q_OBJECT
public:
    WebsiteLockerWidget(QWidget* parent=0);
private slots:
    void readUserData(int);
//    void on_deleteWebsiteButton_clicked();
    void updateUsersModel();
    void on_addWebsiteButton_clicked();
    void on_addCategoryButton_clicked();
    void on_saveButton_clicked();
    void on_deleteButton_clicked();
    void on_resetButton_clicked();
    void on_mergeButton_clicked();
    void on_updateButton_clicked();
    void changeList(bool isChecked);
    void on_expandButton_clicked();
    void on_colapseButton_clicked();
    void finishUpdate(UpdateResult result);
    void updateFiles(UpdateResult result);
private:
    void createModel();
    void initView();
    void save(QString url);
    int userId();
    ListType readUserListType(int userId);
    void writeUserListType(ListType type);
    bool saveChanges();
    bool saveDialog();

    Ui::WebsiteLockerWidget* ui;
    WebsiteLocker::TreeModel* model;
    QSqlTableModel* userNameComboBoxModel;
    QString blockSiteTitle;
};

#endif // WEBSITELOCKERWIDGET_H
