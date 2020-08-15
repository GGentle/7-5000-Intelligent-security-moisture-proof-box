#include "vedio.h"
#include "ui_vedio.h"

vedio::vedio(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::vedio)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);

    finger = new FGport("/dev/ttySAC1",QextSerialBase::Polling);//ARM-ttySAC*,VM-ttyS*
    mycom = new Posix_QextSerialPort("/dev/ttySAC2",QextSerialBase::Polling);//ARM-ttySAC*,VM-ttyS*

    dev_name = "/dev/video0";
    io = IO_METHOD_MMAP;
    fd  = -1;
    buffers= NULL;
    n_buffers= 0;    

    this->setGeometry(50,50,500,440);
    ui->label->setGeometry(50,40,400,300);
    ui->pushButton->setGeometry(50,360,98,27);

    server = new QTcpServer();
    server->listen(QHostAddress::Any, 6665);
    connect(server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    hasclient = false;

    timer = new QTimer(this);
    timer->start(40);
    connect(timer,SIGNAL(timeout()),this,SLOT(sendvedio()));

    opencom();

    sensorsdata = "00000";

    cxvidenc = new CXvidEnc();

    fgtimer = new QTimer(this);
    
//    open_device();
//    init_device();
//    start_capturing();
//    mainloop();
//    stop_capturing();
//    uninit_device();
//    close_device();
//    exit(EXIT_SUCCESS);
   
}

vedio::~vedio()
{
    stop_capturing();
    uninit_device();
    close_device();
    exit(EXIT_SUCCESS);
    cxvidenc->Close();
    delete cxvidenc;

    delete ui;
}

void vedio::changeEvent(QEvent *e)
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

void vedio::run()
{
    open_device();
    init_device();
    start_capturing();
    //    exit(EXIT_SUCCESS);
}


void vedio::open_device()
{
    fd = open (dev_name, O_RDWR /* required */ | O_NONBLOCK, 0); 
    
    if (-1 == fd) 
    { 
        fprintf (stderr, "Cannot open '%s': %d, %s\n",
                dev_name, errno, strerror (errno)); 
        exit (EXIT_FAILURE); 
    } 
}

void vedio::init_device()
{
    struct v4l2_capability cap;
    struct v4l2_cropcap cropcap;
    struct v4l2_crop crop;
    struct v4l2_format fmt;
    unsigned int min;

    if( -1 == ioctl (fd, VIDIOC_QUERYCAP, &cap)){
        qDebug()<<"VIDIOC_QUERYCAP error";
        exit (EXIT_FAILURE);
    }
    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)){
        qDebug()<<dev_name<<"is no video capture device\n";
        exit (EXIT_FAILURE);
    }
    if (!(cap.capabilities & V4L2_CAP_STREAMING)){
        qDebug()<<dev_name<<"does not support streaming i/o\n";
        exit (EXIT_FAILURE);
    }

    //CLEAR (cropcap);
    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (0 == ioctl (fd, VIDIOC_CROPCAP, &cropcap)){
        crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        crop.c = cropcap.defrect; /* reset to default */
        if (-1 == ioctl (fd, VIDIOC_S_CROP, &crop)){
            qDebug()<<"VIDIOC_S_CROP error";
            exit (EXIT_FAILURE);
        }
    }
    else{
        //ignored error;
    }
    CLEAR (fmt);


    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = 640;
    fmt.fmt.pix.height      = 480;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;


    if (-1 == ioctl (fd, VIDIOC_S_FMT, &fmt)){
        qDebug()<<"VIDIOC_S_FMT error";
        exit (EXIT_FAILURE);
    }/* Note VIDIOC_S_FMT may change width and height. */
    /* Buggy driver paranoia. */
    min = fmt.fmt.pix.width * 2;
    if (fmt.fmt.pix.bytesperline < min)
            fmt.fmt.pix.bytesperline = min;
    min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
    if (fmt.fmt.pix.sizeimage < min)
            fmt.fmt.pix.sizeimage = min;

    init_mmap ();


    if (0 == ioctl (fd, VIDIOC_G_FMT, &fmt)){
        qDebug()<<fmt.fmt.pix.width<<"\n";
        qDebug()<<fmt.fmt.pix.height<<"\n";
    }
//    struct v4l2_fmtdesc fmtd;
//
//    memset(&fmtd, 0, sizeof(fmtd));
//
//           fmtd.index = 0;
//
//           fmtd.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
//
//           while(0 == ioctl(fd, VIDIOC_ENUM_FMT, &fmtd)) {
//
//                  fmtd.index++;
//
//                  printf("{ pixelformat = ''%c%c%c%c'', description = ''%s'' }\n",
//
//                                fmtd.pixelformat & 0xFF, (fmtd.pixelformat >> 8) & 0xFF,
//
//                                (fmtd.pixelformat >> 16) & 0xFF, (fmtd.pixelformat >> 24) & 0xFF,
//
//                                fmtd.description);
//
//           }
}

void vedio::init_mmap()
{
    struct v4l2_requestbuffers req;

    CLEAR (req);

    req.count               = 4;
    req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory              = V4L2_MEMORY_MMAP;

    if (-1 == ioctl (fd, VIDIOC_REQBUFS, &req)) {
        qDebug()<<"VIDIOC_REQBUFS error";
        exit (EXIT_FAILURE);
    }

    if (req.count < 2) {
            fprintf (stderr, "Insufficient buffer memory on %s\n",
                     dev_name);
            exit (EXIT_FAILURE);
    }

    buffers = (buffer *)calloc (req.count, sizeof (*buffers));

    if (!buffers) {
            fprintf (stderr, "Out of memory\n");
            exit (EXIT_FAILURE);
    }

    for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
            struct v4l2_buffer buf;

            CLEAR (buf);

            buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory      = V4L2_MEMORY_MMAP;
            buf.index       = n_buffers;

            if (-1 == ioctl (fd, VIDIOC_QUERYBUF, &buf)){
                qDebug()<<"VIDIOC_QUERYBUF error";
                exit (EXIT_FAILURE);
            }


            buffers[n_buffers].length = buf.length;
            buffers[n_buffers].start =
                    mmap (NULL /* start anywhere */,

                          buf.length,
                          PROT_READ | PROT_WRITE /* required */,
                          MAP_SHARED /* recommended */,
                          fd, buf.m.offset);

            if (MAP_FAILED == buffers[n_buffers].start){
                qDebug()<<"mmap error";
                exit (EXIT_FAILURE);
            }
    }
}

void vedio::start_capturing()
{
    unsigned int i;
    enum v4l2_buf_type type;

    for (i = 0; i < n_buffers; ++i) {
            struct v4l2_buffer buf;

            CLEAR (buf);

            buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory      = V4L2_MEMORY_MMAP;
            buf.index       = i;

            if (-1 == ioctl (fd, VIDIOC_QBUF, &buf)){
                qDebug()<<"VIDIOC_QBUF error";
                exit (EXIT_FAILURE);
            }
    }

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (-1 == ioctl (fd, VIDIOC_STREAMON, &type)){
        qDebug()<<"VIDIOC_STREAMON error";
        exit (EXIT_FAILURE);
    }
}

void vedio::mainloop()
{
    unsigned int count;
    count = 1;
    while (count-- > 0) {
            for (;;) {
                    fd_set fds;
                    struct timeval tv;
                    int r;
                    FD_ZERO(&fds);
                    FD_SET(fd, &fds);
                    /* Timeout. */
                    tv.tv_sec = 1;
                    tv.tv_usec = 0;
                    r = select (fd + 1, &fds, NULL, NULL, &tv);



                      if (-1 == r) {
                              if (EINTR == errno)
                                      continue;

                              qDebug()<<"select error";
                              exit (EXIT_FAILURE);
                      }

                      if (0 == r) {
                          qDebug()<<"select timeout";
                          //exit (EXIT_FAILURE);
                      }

                      if (read_frame ())
                              break;

                      /* EAGAIN - continue select loop. */
              }
      }
}

int vedio::read_frame()
{
    struct v4l2_buffer buf;

    CLEAR (buf);

    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;

    if (-1 == ioctl (fd, VIDIOC_DQBUF, &buf)) {
            switch (errno) {
            case EAGAIN:
                    return 0;

            case EIO:
                    break;
                    /* Could ignore EIO, see spec. */

                    /* fall through */

            default:
                    qDebug()<<"VIDIOC_DQBUF error";
                    exit (EXIT_FAILURE);
            }
    }

    assert (buf.index < n_buffers);

    process_image (buffers[buf.index].start,buffers[buf.index].length);

    if (-1 == ioctl (fd, VIDIOC_QBUF, &buf)){
        qDebug()<<"VIDIOC_DQBUF error";
        exit (EXIT_FAILURE);
    }

    return 1;
}

void vedio::process_image(const void *p, int len)
{
    unsigned char *rep;
    rep=(unsigned  char *)p;

//    //test
//    QByteArray testby((char*)rep);
//    int bytelen = testby.length();
//    qDebug()<<len;
//    cxvidenc->AttachCaller(320,240,this);
//    cxvidenc->Open();
//    cxvidenc->Encode(rep);
//    //test

    QPalette palette;
    QImage bmpBuf = QImage::fromData (rep, len);

    if(hasclient){
        QByteArray by;
        QBuffer buffer(&by);
        buffer.open(QIODevice::WriteOnly);

        bool bOK = bmpBuf.save(&buffer ,"jpg", 20);

        len=by.length();
        //qDebug()<<len;
        by.prepend(formHead(QByteArray::number((qlonglong)len)));
        //qDebug()<<sensorsdata;
        by.prepend(sensorsdata);
        clientConnetion->write(by);
    }

    bmpBuf = bmpBuf.scaled(ui->label->width(),ui->label->height());

    palette.setBrush(QPalette::Background, QBrush(bmpBuf));

    ui->label->setAutoFillBackground(true);
    ui->label->setPalette(palette);
    //ui->label->show();

}

void vedio::stop_capturing()
{
    enum v4l2_buf_type type;

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (-1 == ioctl (fd, VIDIOC_STREAMOFF, &type)){
        qDebug()<<"VIDIOC_STREAMOFF error";
        exit (EXIT_FAILURE);
    }
}

void vedio::uninit_device()
{
    unsigned int i;

    for (i = 0; i < n_buffers; ++i)
            if (-1 == munmap (buffers[i].start, buffers[i].length)){
        qDebug()<<"munmap error";
        exit (EXIT_FAILURE);
    }

    free (buffers);
}

void vedio::close_device()
{
    if (-1 == ::close (fd)){
        qDebug()<<"close error";
        exit (EXIT_FAILURE);
    } 
    fd = -1; 
}

//将数据长度封装在10个字节的范围内,不足的在前面补零
QByteArray vedio::formHead(QByteArray by){
    int len = by.length();
    int i = 0;
    for(i = 10 - len;i > 0;i--){
       by.prepend('0');
    }
//    //sensorsdata
//    if(5 == sensorsdata.length())
//        by.prepend(sensorsdata);
//    else{
//        for(i = 0;i < 5;i++){
//            by.prepend('0');
//        }
//    }
    return by;
}

void vedio::sendvedio()
{
    mainloop();
}

void vedio::acceptConnection()
{
    clientConnetion = server->nextPendingConnection();
    connect(clientConnetion, SIGNAL(readyRead()), this, SLOT(readClient()));
    hasclient = true;
    qDebug()<<"1";
}

void vedio::readClient()
{
    QByteArray by;

    by = clientConnetion->readAll();

    if(QByteArray("get") == by){
        qDebug()<<"get";
        finger->getFG();
    }
    else if(QByteArray("check") == by){
        qDebug()<<"check";
        finger->waitCheck();
        if(WaitingCheck == finger->getfgrst())
        {
            fgtimer->start(500);
            connect(fgtimer,SIGNAL(timeout()),this,SLOT(waitfgresult()));
        }
    }
    else{
        char a,b;
        a = by[1];
        b = by[2];
        qDebug()<<a<<b;
        by.append("\r\n");
        by.prepend("st:");
        mycom->write(by);
    }
}

void vedio::on_pushButton_clicked()
{

}

void vedio::readmycom()
{
    QByteArray temp = mycom->readAll();
    char ssdata;

    //qDebug()<<temp[0];
    if("start\r\n" == temp.left(7) && temp.length() == 23){
        qDebug()<<"sensorsdata gotten";
        temp.remove(0,7);
        sensorsdata = temp.left(5);//sensors
        ssdata = sensorsdata[1];
//        if(ssdata & 0x80)
//            qDebug()<<'1';
//        else
//            qDebug()<<'0';
//        if(ssdata & 0x40)
//            qDebug()<<'1';
//        else
//            qDebug()<<'0';
//        if(ssdata & 0x20)
//            qDebug()<<'1';
//        else
//            qDebug()<<'0';
//        if(ssdata & 0x10)
//            qDebug()<<'1';
//        else
//            qDebug()<<'0';
//        if(ssdata & 0x08)
//            qDebug()<<'1';
//        else
//            qDebug()<<'0';
//        if(ssdata & 0x04)
//            qDebug()<<'1';
//        else
//            qDebug()<<'0';
//        if(ssdata & 0x02)
//            qDebug()<<'1';
//        else
//            qDebug()<<'0';
//        if(ssdata & 0x01)
//            qDebug()<<'1';
//        else
//            qDebug()<<'0';

//        temp.remove(0,5);
//        if("\r\n" == temp.left(2)){
//            temp.remove(0,2);
//            temp.remove(0,2);
//            if(!("\r\nend\r\n" == temp)){
//                //wrong
//            }
//        }
//        else{
//            //wrong
//        }
    }
    else{
        qDebug()<<"no serialport messege";
    }

//    //check the door for duoji
//    if(ssdata & 0x40)
//        finger->closedoor();
//
//    //check people for checkFG
//    if(ssdata & 0x10)
//        finger->waitCheck();
//    else
//        finger->stopCheck();
}

void vedio::opencom()
{
    mycom->open(QIODevice::ReadWrite);
    mycom->setBaudRate(BAUD9600);
    mycom->setDataBits(DATA_8);
    mycom->setParity(PAR_NONE);
    mycom->setStopBits(STOP_1);
    mycom->setFlowControl(FLOW_OFF);
    mycom->setTimeout(10);

    comtimer = new QTimer(this);
    comtimer -> start(100);
    connect(comtimer,SIGNAL(timeout()),this,SLOT(readmycom()));
}

void vedio::encode(unsigned char *image)
{
    printf("g3b\n");
    cxvidenc->AttachCaller(640,480,this);
    cxvidenc->Open();
    cxvidenc->Encode(image);

}

void vedio::PostEncHandler(unsigned char *xvid, int key, int xvid_len)
{
    qDebug()<<xvid_len;
}

void vedio::waitfgresult()
{
    QByteArray by;
    if(RightCheck == finger->getfgrst())
    {
        qDebug()<<"send to com h0";
        char a,b;
        a = 'h';
        b = 0x00;
        qDebug()<<a<<b;
        by.append("\r\n");
        by.prepend("st:");
        mycom->write(by);
        disconnect(fgtimer,SIGNAL(timeout()),this,SLOT(waitfgresult()));
    }

    if(WrongCheck == finger->getfgrst())
    {
        disconnect(fgtimer,SIGNAL(timeout()),this,SLOT(waitfgresult()));
    }
}
