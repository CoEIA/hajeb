#ifndef SCREENSHOTWIDGET_H
#define SCREENSHOTWIDGET_H

#include <QWidget>
#include <QFileInfoList>

namespace Ui {
    class ScreenshotWidget;
}

class QSqlTableModel;

class ScreenshotWidget : public QWidget {
    Q_OBJECT
public:
    ScreenshotWidget(QWidget* parent=0);
private slots:
    void on_nextPageButton_clicked();
    void on_previousPageButton_clicked();
    void on_updateScreenshotButton_clicked();
    void updateScreenshot();
    void on_clearScreenshotButton_clicked();
    void on_saveButton_clicked();
    void readUserScreenshot(int index);
    void readUserScreenshotTime(int index);
    void updateUsersModel();
    void updateImageListView(int pageNo);
signals:
    void pageChanged(int);
private:
    void createModel();
    void initListWidget();
    QFileInfoList getPageImage(int pageNo);
    void setCurrentPage(int page);
    Ui::ScreenshotWidget* ui;
    QSqlTableModel* userNameComboBoxModel;
    QString screenshotTitle;
    QFileInfoList imageList;
    int m_totalPages;
    const int MAX_IMG;
    int m_currentPage;
};

#endif // SCREENSHOTWIDGET_H
