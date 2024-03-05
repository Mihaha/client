#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QMessageBox>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
        QTcpSocket* socket;
    QByteArray Data;
    QTimer* timer;
    QJsonDocument doc;
    QJsonParseError docError;

public slots:
    void sockReady();
    void sockDisk();
    void onTimeOut();


private slots:
    void on_pushButton_clicked();

    //void on_Freq_windowIconTextChanged(const QString &iconText);

    void on_pushButton_2_clicked();

    void on_pushButton_crateGPIO_clicked();

    void on_pushButton_changeModeGPIO_clicked();

    //void on_pushButton_3_clicked();

    //void on_pushButton_chekModeGPIO_clicked();

    void on_pushButton_chekMode_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
