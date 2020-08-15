/****************************************************************************
** Meta object code from reading C++ file 'vedio.h'
**
** Created: Thu Mar 17 15:14:57 2016
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "vedio.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vedio.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_vedio[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
       7,    6,    6,    6, 0x08,
      31,    6,    6,    6, 0x08,
      43,    6,    6,    6, 0x08,
      62,    6,    6,    6, 0x08,
      75,    6,    6,    6, 0x08,
      90,    6,    6,    6, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_vedio[] = {
    "vedio\0\0on_pushButton_clicked()\0"
    "sendvedio()\0acceptConnection()\0"
    "readClient()\0waitfgresult()\0readmycom()\0"
};

const QMetaObject vedio::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_vedio,
      qt_meta_data_vedio, 0 }
};

const QMetaObject *vedio::metaObject() const
{
    return &staticMetaObject;
}

void *vedio::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_vedio))
        return static_cast<void*>(const_cast< vedio*>(this));
    if (!strcmp(_clname, "CXvidEncHandler"))
        return static_cast< CXvidEncHandler*>(const_cast< vedio*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int vedio::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_pushButton_clicked(); break;
        case 1: sendvedio(); break;
        case 2: acceptConnection(); break;
        case 3: readClient(); break;
        case 4: waitfgresult(); break;
        case 5: readmycom(); break;
        }
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
