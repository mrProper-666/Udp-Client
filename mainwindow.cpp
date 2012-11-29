#include "mainwindow.h"
#include "ui_mainwindow.h"

#define sendPort 6666

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(qApp->applicationName() + ' ' + qApp->applicationVersion());
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::Any, sendPort);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(errorConnection(QAbstractSocket::SocketError)));
    ui->lineEdit->setInputMask("000.000.000.000:0000");
    QRegExpValidator *validator = new QRegExpValidator(QRegExp("^[0-9\\s]*$"), this);
    ui->lineEdit_2->setValidator(validator);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectToServer(){

        port = ui->lineEdit->text().remove(0,ui->lineEdit->text().indexOf(":")+1).toInt();

        addr.setAddress(ui->lineEdit->text().remove(ui->lineEdit->text().indexOf(":"),
                                                    ui->lineEdit->text().length()));

        if (addr.isNull()) {
            QMessageBox::critical(this,qApp->applicationName(),
                                  tr("Ошибка подключения. Введите корректный адрес\n"
                                     "например 127.0.0.1:666"),
                                  QMessageBox::Ok);
        }else{
            ui->listWidget->addItem(QString("%1 Адрес сохранен %2:%3")
                                    .arg(QDateTime::currentDateTime().toString(Qt::SystemLocaleShortDate))
                                    .arg(addr.toString())
                                    .arg(port));
            ui->pushButton->setEnabled(false);
            ui->pushButton_2->setEnabled(true);
        }
}

void MainWindow::on_pushButton_clicked()
{
    connectToServer();
}

void MainWindow::errorConnection(QAbstractSocket::SocketError e){

    if (socket->state() == QAbstractSocket::ConnectingState){
        socket->disconnectFromHost();
    } else {
        return;
    }
    ui->pushButton->setEnabled(true);
    QMessageBox::critical(this,qApp->applicationName(),
                          tr("Ошибка подключения\n Возможно сервер не отвечает, или не запущен\n" +
                             e),
                          QMessageBox::Ok);
}

void MainWindow::readData(){

    while (socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        socket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        qDebug() << datagram;
        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
        item->setText(QString("%1 >> %2")
                   .arg(QDateTime::currentDateTime().toString(Qt::SystemLocaleShortDate))
                   .arg(QString(datagram)));
        item->setBackgroundColor(QColor(Qt::green));

        ui->listWidget->addItem(item);
    }
}

void MainWindow::sendString(QString string){
    string.append(tr("@%1").arg(sendPort));
    QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
    item->setText(QString("%1 << %2")
               .arg(QDateTime::currentDateTime().toString(Qt::SystemLocaleShortDate))
               .arg(string));
    item->setBackgroundColor(QColor(Qt::blue));

    ui->listWidget->addItem(item);

    socket->writeDatagram(string.toUtf8(), addr, port);
}

void MainWindow::on_pushButton_2_clicked()
{
    sendString(ui->lineEdit_2->text());
}
