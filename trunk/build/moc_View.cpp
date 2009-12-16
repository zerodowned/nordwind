/****************************************************************************
** Meta object code from reading C++ file 'View.hpp'
**
** Created: Thu 17. Dec 00:29:08 2009
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/game/View.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'View.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_game__View[] = {

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

static const char qt_meta_stringdata_game__View[] = {
    "game::View\0"
};

const QMetaObject game::View::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_game__View,
      qt_meta_data_game__View, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &game::View::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *game::View::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *game::View::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_game__View))
        return static_cast<void*>(const_cast< View*>(this));
    return QGraphicsView::qt_metacast(_clname);
}

int game::View::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
