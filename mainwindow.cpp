#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()),this,SLOT(sockReady()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(sockDisk()));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    socket->connectToHost("192.168.0.136", 5555);
}
void MainWindow::sockDisk()
{
    socket->deleteLater();
}
void MainWindow::sockReady()
{
    if(socket->waitForConnected(500))
    {
        Data = socket->readAll();

        doc = QJsonDocument::fromBinaryData(Data);

        QJsonObject obj = doc.object();
        ui->Freq->setText(QString::number(obj["freq"].toInt())+" Hz");
        ui->Temp->setText(QString::number(obj["temp"].toDouble())+" °C");
        if(doc.object().value("GPIOReady").toString()=="yes")
        {
            QStandardItemModel* model = new QStandardItemModel(nullptr);
            model->setHorizontalHeaderLabels(QStringList()<<"Availabel GPIO");
            QString gpio = doc.object().value("availableGPIO").toString();
            QStringList listGPIO = gpio.split(',');

            for(int i=0; i<listGPIO.length()-1; i++)
            {
                QStandardItem* col = new QStandardItem(listGPIO[i]);
                model->appendRow(col);
            }

            ui->tableView->setModel(model);
        }
        else if(doc.object().value("chekMode").toString()=="GPIO")
        {
            ui->lineEdit_chekActiveLow->setText(doc.object().value("activeLow").toString());
            ui->lineEdit_chekDirection->setText(doc.object().value("direction").toString());
            ui->lineEdit_chekValue->setText(doc.object().value("value").toString());
        }
        else if(doc.object().value("statusGPIO").toString()=="closed")
        {
            QMessageBox::information(this,"Информация","GPIO с таким номером не идентифицирован");
        }
    }
}

void MainWindow::onTimeOut()
{
    if(socket->isOpen())
    {
        QJsonObject freqTemp;
        freqTemp.insert("type","request");
        freqTemp.insert("value","freqTemp");
        QJsonDocument freqTempDoc;
        freqTempDoc.setObject(freqTemp);
        socket->write(freqTempDoc.toBinaryData());
    }
}



void MainWindow::on_pushButton_2_clicked()
{
    if (socket->isOpen())
    {
        QJsonObject obj;
        obj.insert("type","request");
        obj.insert("value","GPIO");
        QJsonDocument dc;
        dc.setObject(obj);
        socket->write(dc.toBinaryData());
    }
}


void MainWindow::on_pushButton_crateGPIO_clicked()
{
    if (socket->isOpen())
    {
        QString st = ui->lineEdit_5->text();
        QJsonObject jsonCreateGPIO;
        jsonCreateGPIO.insert("type","create");
        jsonCreateGPIO.insert("value",st);
        QJsonDocument createGPIO;
        createGPIO.setObject(jsonCreateGPIO);
        socket->write(createGPIO.toBinaryData());

    }
}


void MainWindow::on_pushButton_changeModeGPIO_clicked()
{
    if (socket->isOpen())
    {
        QString numberGPIO = ui->lineEdit_modeNumberGPIO->text();
        QString direction = ui->lineEdit_modeDirection->text();
        QString value = ui->lineEdit_modeValue->text();
        QString activeLow = ui->lineEdit_modeActiveLow->text();
        QJsonObject changeModeGPIO;
        changeModeGPIO.insert("type","changeMode");
        changeModeGPIO.insert("numberGPIO",numberGPIO);
        changeModeGPIO.insert("direction",direction);
        changeModeGPIO.insert("value",value);
        changeModeGPIO.insert("activeLow",activeLow);
        QJsonDocument changeDoc;
        changeDoc.setObject(changeModeGPIO);
        socket->write(changeDoc.toBinaryData());
    }
}




void MainWindow::on_pushButton_chekMode_clicked()
{
    if(socket->isOpen())
    {
        QString numberGPIO = ui->lineEdit_chekNumberGPIO->text();
        QJsonObject chekMode;
        chekMode.insert("type","chekMode");
        chekMode.insert("numberGPIO",numberGPIO);
        QJsonDocument chekDoc;
        chekDoc.setObject(chekMode);
        socket->write(chekDoc.toBinaryData());
    }
}

