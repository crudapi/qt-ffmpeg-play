#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "device.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void init();
    void capture(string deviceName);

private slots:
    void on_startButton_clicked();

    void on_stopButton_clicked();

private:
    Ui::MainWindow *ui;
    Device* m_device;
};
#endif // MAINWINDOW_H
