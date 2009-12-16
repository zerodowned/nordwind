/****************************************************************************
** Meta object code from reading C++ file 'UserInterface.hpp'
**
** Created: Thu 17. Dec 00:36:34 2009
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/core/UserInterface.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'UserInterface.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_core__UserInterfaces[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_core__UserInterfaces[] = {
    "core::UserInterfaces\0"
};

const QMetaObject core::UserInterfaces::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_core__UserInterfaces,
      qt_meta_data_core__UserInterfaces, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &core::UserInterfaces::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *core::UserInterfaces::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *core::UserInterfaces::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_core__UserInterfaces))
        return static_cast<void*>(const_cast< UserInterfaces*>(this));
    if (!strcmp(_clname, "Singleton<core::UserInterfaces>"))
        return static_cast< Singleton<core::UserInterfaces>*>(const_cast< UserInterfaces*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int core::UserInterfaces::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
