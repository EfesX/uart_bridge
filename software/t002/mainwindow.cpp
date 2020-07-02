#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "iostream"

#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QTextDocumentWriter>
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    on_pushButton_3_clicked();

    connect(&comport, SIGNAL(readyRead()), this, SLOT(com_read()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_wrBtn_clicked()
{
    if(!comport.isOpen()){
        QMessageBox::warning(this, "Ошибка", "Выберите COM порт и нажмите кнопку Connect");
    }else{
        QString dev = ui->addrDev->displayText();
        QString reg = ui->addrReg->displayText();
        QString cnt = ui->cntByte->displayText();


        QString dataq = QString('W') + QString(' ') + dev + QString(' ') + reg + QString(' ') + cnt;


        QString data(ui->textBrowser->document()->toPlainText().toUtf8());
        if(data[data.length()] != '\n'){data += QString('\n');}

        dataq += data;

        //qDebug() << dataq.toUtf8();
        comport.write(dataq.toUtf8());
    }
}

void MainWindow::on_pushButton_3_clicked()
{

    QString name;
    ui->comboBox->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        name = info.portName();
        ui->comboBox->addItem(name);
    }

}

void MainWindow::on_pushButton_4_clicked()
{
    ui->textBrowser->clear();

}


void MainWindow::on_connectBtn_clicked()
{
    if(comport.isOpen()){
        comport.close();
        qDebug() << "COM was closed";
        ui->connectBtn->setText("Connect");
    }else{
        comport.setPortName(ui->comboBox->currentText());
        comport.setBaudRate((qint32) ui->comboBox_baudRate->currentText().toInt());
        comport.setDataBits(QSerialPort::DataBits::Data8);
        comport.setParity(QSerialPort::Parity::NoParity);
        comport.setStopBits(QSerialPort::StopBits::OneStop);
        comport.setFlowControl(QSerialPort::FlowControl::NoFlowControl);

        if(comport.open(QIODevice::ReadWrite)){
            qDebug() << "COM was open";
            ui->connectBtn->setText("Disconnect");
        }else qDebug() << "open error";
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, QString::fromUtf8("Открыть файл"), QDir::currentPath(), "Text (*.txt);;All files (*.*)");

    QFile file(fileName);

    if ((file.exists())&&(file.open(QIODevice::ReadOnly)))
    {
        ui->textBrowser->setText(file.readAll());
        file.close();
    }
}

void MainWindow::on_btnSaveFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, QString::fromUtf8("Открыть файл"), QDir::currentPath(), "Text (*.txt);;All files (*.*)");

    QFile file(fileName);

    if (file.open(QIODevice::WriteOnly))
    {
        QTextDocumentWriter writer(fileName, "plaintext");
        writer.write(ui->textBrowser->document());
    } file.close();
}

void MainWindow::com_read(){
    //ui->textBrowser->clear();
    QString temp(ui->textBrowser->document()->toPlainText());
    temp += comport.readAll();
    ui->textBrowser->setText(temp);

    qDebug() << comport.readAll();
}

void MainWindow::on_pushButton_8_clicked()
{
    if(!comport.isOpen()){
        QMessageBox::warning(this, "Ошибка", "Выберите COM порт и нажмите кнопку Connect");
    }else{
        QString dev = ui->addrDev->displayText();
        QString reg = ui->addrReg->displayText();
        QString cnt = ui->cntByte->displayText();

        QString dataq = QString('R') + QString(' ') + dev + QString(' ') + reg + QString(' ') + cnt;

        comport.write(dataq.toUtf8());
    }
}
