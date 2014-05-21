#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

//class HeaderWidget;
class QLabel;
class QStackedWidget;
class QSignalMapper;
class SlidingStackedWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum MenuButton {
        Home,
        UserSettings,
        ServiceLog,
        ScreenshotMonitor,
        SurfTime,
        WebsiteLocker,
        ProgramLocker
    };

    MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);
private slots:
    void setCurrentWindow(int id);
    void readAvailableDays();
signals:
    void updateCurrentDateTime();
    void updateLogs();
    void updateScreenshots();
    void updateStatistics();
private:
    void createDockWidget();
    void createStatusBar();
    SlidingStackedWidget* stackedWidget;
    //HeaderWidget* headerWidget;
    QWidget* headerWidget;
    QWidget* rightPanelWidget;
    QLabel* currentPageLabel;
    QLabel* versionLabel;
    QSignalMapper* signalMapper;
};

#endif // MAINWINDOW_H
