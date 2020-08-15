#ifndef VEDIORCV_H
#define VEDIORCV_H

#include <QMainWindow>
#include <QtNetwork>

namespace Ui {
    class vediorcv;
}

class vediorcv : public QMainWindow {
    Q_OBJECT
public:
    vediorcv(QWidget *parent = 0);
    ~vediorcv();

protected:
    void changeEvent(QEvent *e);

public slots:
    void acceptConnection();
    void readClient();

private:
    Ui::vediorcv *ui;

    QTcpServer *server;
    QTcpSocket *clientConnection;

};

#endif // VEDIORCV_H
