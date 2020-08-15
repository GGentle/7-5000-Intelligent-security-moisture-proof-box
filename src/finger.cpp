#include "finger.h"
#include <QDebug>


FGport::FGport(const QString & name, QextSerialBase::QueryMode mode) :Posix_QextSerialPort(name,mode)
{
    ISTA = None;
    i = 0;

    readTimer = new QTimer(this);

    readTimer -> start(500);


    timer = new QTimer(this);

    timer -> start(2000);


//    fd = ::open("/dev/duoji",O_RDWR);
//    if(fd < 0)
//    {
//        perror("open device CWL_duoji");
//        exit(1);
//    }

    openFG();

    connect(readTimer,SIGNAL(timeout()),this,SLOT(readmycom()));

}

FGport::~FGport()
{

}

void FGport::readmycom()

{

    QByteArray temp = readAll();
    QString tempstr;


    HEX2Str(temp,tempstr);

    if(!(tempstr.isEmpty())  && (tempstr.left(4)) == "EF01")
    {

        switch(REGSTA)
        {
            case WaitInput:
                if(tempstr == "EF01FFFFFFFF07000300000A")
                {

                    getfeature();
                    makefeature();
                    if(WaitingGet == _FGGetState)
                    {
                        REGSTA = WaitFiger;
                        disconnect(timer,SIGNAL(timeout()),this,SLOT(getimage()));
                        qDebug()<<"Please put your finger to the sensor\n";
                        _FGGetState = Gotton;
                    }
                    else if(Gotton == _FGGetState)
                    {
                        REGSTA = WaitCompr;
                        qDebug()<<"Got finger\n";
                    }
                }
                break;
            case WaitFiger:
                if("EF01FFFFFFFF07000300000A" == tempstr)
                {
                    qDebug("2");
                    getimage();
                    REGSTA = WaitInput2;
                }
                break;
            case WaitInput2:
                if("EF01FFFFFFFF07000300000A" == tempstr)
                {
                    qDebug("3");
                    getfeature();
                    makefeature();
                    REGSTA = WaitModel;
                }
                break;
            case WaitModel:
                if("EF01FFFFFFFF07000300000A" == tempstr)
                {
                    getmodel();
                }
                REGSTA = WaitModel2;
                break;
            case WaitModel2:
                if("EF01FFFFFFFF07000300000A" == tempstr)savemodel();
                REGSTA = WaitResult;
                break;
            case WaitResult:
                if("EF01FFFFFFFF07000300000A" == tempstr);
                qDebug()<<"Finger Saved\n";
                REGSTA = WaitInput;
                disconnect(timer,SIGNAL(timeout()),this,SLOT(getimage()));
                break;
            case WaitCompr:
                if("EF01FFFFFFFF07000300000A" == tempstr)loadmodel();
                REGSTA = WaitLoad;
                break;
            case WaitLoad:
                if("EF01FFFFFFFF07000300000A" == tempstr)matchmodel();
                REGSTA = WaitRes_cmp;
                break;
            case WaitRes_cmp:
                if("EF01FFFFFFFF07000500" == tempstr.left(20))
                {
                    qDebug()<<"The door is open!\n";
                    ioctl(fd,0,1000);
                    _FGCheckState = RightCheck;

                }
                if("EF01FFFFFFFF07000508" == tempstr.left(20))
                {
                    qDebug()<<"Wrong finger!\n";
                    printf("Wrong finger!\n");
                    sendtopc("0");
                    _FGCheckState = WrongCheck;
                }
                break;
            default:break;
        }
    }
}

void FGport::openFG()
{
    open(QIODevice::ReadWrite);
    setBaudRate(BAUD9600);
    setDataBits(DATA_8);
    setParity(PAR_NONE);
    setStopBits(STOP_1);
    setFlowControl(FLOW_OFF);
    setTimeout(10);
}

void FGport::waitCheck()
{
    _FGGetState = Gotton;
    _FGCheckState = WaitingCheck;
    REGSTA = WaitInput;

    connect(timer,SIGNAL(timeout()),this,SLOT(getimage()));
}


void FGport::getFG()
{
    _FGGetState = WaitingGet;
    REGSTA = WaitInput;

    connect(timer,SIGNAL(timeout()),this,SLOT(getimage()));
}

void FGport::sendtocom(QString str)
{
    QByteArray temparr;
    Str2HEX(str,temparr);
    write(temparr);
}


char FGport::ConverHexChar(char ch)
{
    if((ch >= '0')&&(ch <= '9')) return ch -0x30;
    else if((ch >= 'A')&&(ch <= 'F')) return ch - 'A' + 10;
    else if((ch >= 'a')&&(ch <= 'f')) return ch - 'a' + 10;
    else return (-1);
}

void FGport::Str2HEX(QString str,QByteArray &senddata)
{
    int hexdata,lowhexdata,hexdatalen = 0,i = 0;
    int len = str.length();
    char lstr,hstr;

    senddata.resize(len/2);

    while(i<len)
    {
        hstr = str[i].toAscii();

        if(hstr == ' ')
        {
            i++;continue;
        }

        i++;

        if(i >= len) break;

        lstr = str[i].toAscii();
        hexdata = ConverHexChar(hstr);
        lowhexdata = ConverHexChar(lstr);
        if((hexdata == -1) || (lowhexdata) == -1)break;
        else hexdata = hexdata * 16 + lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);

}

void FGport::HEX2Str(QByteArray arr,QString &recivedata)
{
    unsigned char str,len;
    QString hstr,lstr;
    char
    ANSIIhex[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

    len = arr.size();
    while(len--)
    {
      str = arr[0];
      arr.remove(0,1);
      hstr = ANSIIhex[str / 16];
      lstr = ANSIIhex[str % 16];
      recivedata += hstr + lstr;
    }
}

void FGport::getimage()
{
    sendtocom(SFG_GetImage);
}
void FGport::makefeature()
{
    if(REGSTA == WaitInput)
    {
        sendtocom(SFG_GenChar1);
    }
    else if(REGSTA == WaitInput2)
    {
        sendtocom(SFG_GenChar2);
    }
}
void FGport::getmodel()
{
    sendtocom(SFG_RegModel);
}
void FGport::savemodel()
{
    sendtocom(SFG_StoreChar);
}
void FGport::loadmodel()
{
    sendtocom(SFG_LoadChar);
}
void FGport::matchmodel()
{
    sendtocom(SFG_Match);
}
void FGport::getfeature()
{
    sendtocom(SFG_GenBinImage0);
    sendtocom(SFG_GenBinImage1);
    sendtocom(SFG_GenBinImage2);
}

void FGport::sendtopc(QString str)
{
    QByteArray ba = str.toLocal8Bit();
    char *ch = ba.data();

}

void FGport::closedoor()
{
    ioctl(fd,0,500);
}

void FGport::stopCheck()
{
    disconnect(timer,SIGNAL(timeout()),this,SLOT(getimage()));
}

FGCheckState FGport::getfgrst()
{
    return _FGCheckState;
}
