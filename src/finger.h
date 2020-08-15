#ifndef FINGER_H
#define FINGER_H

#include "posix_qextserialport.h"
#include <QTimer>
#include <QDebug>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>

#define Rand_Num         "EF 01 FF FF FF FF 01 00 03 14 00 18"
#define SFG_GetImage     "EF 01 FF FF FF FF 01 00 03 01 00 05"
#define SFG_GenChar1     "EF 01 FF FF FF FF 01 00 04 02 01 00 08"
#define SFG_GenChar2     "EF 01 FF FF FF FF 01 00 04 02 02 00 09"
#define SFG_Match        "EF 01 FF FF FF FF 01 00 03 03 00 07"
#define SFG_RegModel     "EF 01 FF FF FF FF 01 00 03 05 00 09"
#define SFG_StoreChar    "EF 01 FF FF FF FF 01 00 06 06 01 00 00 00 0e"
#define SFG_LoadChar     "EF 01 FF FF FF FF 01 00 06 07 02 00 00 00 10"
#define SFG_GenBinImage0 "EF 01 FF FF FF FF 01 00 04 1C 00 00 21"
#define SFG_GenBinImage1 "EF 01 FF FF FF FF 01 00 04 1C 01 00 22"
#define SFG_GenBinImage2 "EF 01 FF FF FF FF 01 00 04 1C 02 00 23"

enum REGSTATUS
{
    WaitInput,
    WaitInput2,
    WaitModel,
    WaitModel2,
    WaitResult,
    WaitCompr,
    WaitFiger,
    WaitLoad,
    WaitRes_cmp
};

enum IMGSTATUS
{
    None,
    Origin,
    Bin,
    Detail,
    DetailF
};

enum FGCheckState{
        WaitingCheck,
        WrongCheck,
        RightCheck
};


class FGport: public Posix_QextSerialPort
{
    Q_OBJECT

public:
    enum FGGetState {
            WaitingGet,
            Gotton,
    }_FGGetState;

    FGCheckState _FGCheckState;


    FGport(const QString & name, QextSerialBase::QueryMode mode);
    ~FGport();

    REGSTATUS REGSTA;
    IMGSTATUS ISTA;
    int i,stepcmd,stepround,steplr;
    int fd;

    char ConverHexChar(char ch);
    void Str2HEX(QString str,QByteArray &senddata);
    void HEX2Str(QByteArray arr,QString &recivedata);
    void sendtocom(QString str);
    FGCheckState getfgrst();

    QString dat;
    QString ReciveData;

private:
    QTimer *readTimer;
    QTimer *timer;

public slots:
    void readmycom();
    void getFG();
    void waitCheck();
    void openFG();
    void stopCheck();

    void getimage();
    void makefeature();
    void getmodel();
    void savemodel();
    void loadmodel();
    void matchmodel();
    void getfeature();

    void sendtopc(QString str);

    void closedoor();


};

#endif // FINGER_H
