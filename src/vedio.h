#ifndef VEDIO_H
#define VEDIO_H

#include <QMainWindow>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QLabel>
#include <QTimer>
#include <QByteArray>
#include <QtNetwork>

//serialport
#include "posix_qextserialport.h"

//step motor drive
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>

//vedio
#include "linux.h"
#include <xvid.h>
#include "./CXvidEnc.h"

//finger
#include "finger.h"

#define CLEAR(x) memset (&(x), 0, sizeof (x))

#define CIF_ROW 480
#define CIF_COL 640

namespace Ui {
    class vedio;
}

class vedio : public QMainWindow ,
public CXvidEncHandler  // xvid encode handler
{
    Q_OBJECT
public:
    vedio(QWidget *parent = 0);
    ~vedio();

    void run(void);

    // override the CXvidEncHandler
     void PostEncHandler(unsigned char * xvid, int key, int xvid_len) ;

protected:
    void changeEvent(QEvent *e);

    //getframe
    void open_device(void);
    void init_device(void);
    void init_mmap(void);
    void start_capturing(void);
    void mainloop (void);
    int read_frame(void);
    void process_image (const void *p,int len);
    void stop_capturing(void);
    void uninit_device(void);
    void close_device(void);

    //xvidencode
    void encode(unsigned char *image);

    void opencom();

    QByteArray formHead(QByteArray by);

private:
    Ui::vedio *ui;

    QTcpServer *server;
    QTcpSocket *clientConnetion;
    bool hasclient;
    
    char * dev_name;
    io_method  io;
    int  fd;
    buffer *  buffers;
    unsigned int n_buffers;

    QTimer *timer;

    FGport *finger;
    QTimer *fgtimer;

    QTimer *comtimer;
    Posix_QextSerialPort *mycom;
    QByteArray sensorsdata;

    CXvidEnc *cxvidenc;
private slots:
    void on_pushButton_clicked();
    void sendvedio();
    void acceptConnection();
    void readClient();
    void waitfgresult();

    void readmycom();
};

#endif // VEDIO_H
