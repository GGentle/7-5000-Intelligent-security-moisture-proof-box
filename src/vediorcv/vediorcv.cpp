#include "vediorcv.h"
#include "ui_vediorcv.h"

vediorcv::vediorcv(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::vediorcv)
{
    ui->setupUi(this);

    ui->label->setGeometry(50,40,320,240);

    server = new QTcpServer();
    server->listen(QHostAddress::Any, 6665);
    connect(server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    qDebug("2");
}

vediorcv::~vediorcv()
{
    delete ui;
}

void vediorcv::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void vediorcv::acceptConnection()
{
    qDebug("2");
    clientConnection = server->nextPendingConnection();
    connect(clientConnection, SIGNAL(readyRead()), this, SLOT(readClient()));
}

void vediorcv::readClient()
{
    qDebug("3");
    clientConnection->setReadBufferSize(1024*1024);
    QByteArray imageData = clientConnection->readAll();

    QPalette palette;

    QImage image=QImage::fromData(imageData,"jpg");
    palette.setBrush(QPalette::Background, QBrush(image));


    ui->label->setAutoFillBackground(true);
    ui->label->setPalette(palette);

}
