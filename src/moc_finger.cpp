/****************************************************************************
** Meta object code from reading C++ file 'finger.h'
**
** Created: Thu Mar 17 15:15:01 2016
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "finger.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'finger.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FGport[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x0a,
      20,    7,    7,    7, 0x0a,
      28,    7,    7,    7, 0x0a,
      40,    7,    7,    7, 0x0a,
      49,    7,    7,    7, 0x0a,
      61,    7,    7,    7, 0x0a,
      72,    7,    7,    7, 0x0a,
      86,    7,    7,    7, 0x0a,
      97,    7,    7,    7, 0x0a,
     109,    7,    7,    7, 0x0a,
     121,    7,    7,    7, 0x0a,
     134,    7,    7,    7, 0x0a,
     151,  147,    7,    7, 0x0a,
     169,    7,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FGport[] = {
    "FGport\0\0readmycom()\0getFG()\0waitCheck()\0"
    "openFG()\0stopCheck()\0getimage()\0"
    "makefeature()\0getmodel()\0savemodel()\0"
    "loadmodel()\0matchmodel()\0getfeature()\0"
    "str\0sendtopc(QString)\0closedoor()\0"
};

const QMetaObject FGport::staticMetaObject = {
    { &Posix_QextSerialPort::staticMetaObject, qt_meta_stringdata_FGport,
      qt_meta_data_FGport, 0 }
};

const QMetaObject *FGport::metaObject() const
{
    return &staticMetaObject;
}

void *FGport::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FGport))
        return static_cast<void*>(const_cast< FGport*>(this));
    return Posix_QextSerialPort::qt_metacast(_clname);
}

int FGport::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Posix_QextSerialPort::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: readmycom(); break;
        case 1: getFG(); break;
        case 2: waitCheck(); break;
        case 3: openFG(); break;
        case 4: stopCheck(); break;
        case 5: getimage(); break;
        case 6: makefeature(); break;
        case 7: getmodel(); break;
        case 8: savemodel(); break;
        case 9: loadmodel(); break;
        case 10: matchmodel(); break;
        case 11: getfeature(); break;
        case 12: sendtopc((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: closedoor(); break;
        }
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
