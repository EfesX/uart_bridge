#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QSerialPort comport;

private slots:
    void on_wrBtn_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_connectBtn_clicked();

    void on_pushButton_6_clicked();

    void on_btnSaveFile_clicked();

    void com_read();

    void on_pushButton_8_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
