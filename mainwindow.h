#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QUdpSocket>
#include <QtGui/QMessageBox>
#include <QHostAddress>
#include <QtDebug>
#include <QAbstractSocket>
#include <QDateTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QUdpSocket *socket;
    void connectToServer();
    QHostAddress addr;
    qint16 port;
    void sendString(QString string);

private slots:
    void on_pushButton_clicked();
    void errorConnection(QAbstractSocket::SocketError e);
    void readData();
    void on_pushButton_2_clicked();
};

#endif // MAINWINDOW_H
