/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../QHYGROTHERM-DATA-VIEW/mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      30,   11,   11,   11, 0x08,
      55,   45,   11,   11, 0x08,
     116,  104,   11,   11, 0x08,
     163,   11,   11,   11, 0x08,
     191,   11,   11,   11, 0x08,
     220,   11,   11,   11, 0x08,
     254,   11,   11,   11, 0x08,
     282,   11,   11,   11, 0x08,
     310,   11,   11,   11, 0x08,
     347,   11,   11,   11, 0x08,
     382,  377,   11,   11, 0x08,
     424,  377,   11,   11, 0x08,
     469,   11,   11,   11, 0x08,
     491,   11,   11,   11, 0x08,
     522,   11,   11,   11, 0x08,
     550,   11,   11,   11, 0x08,
     579,  377,   11,   11, 0x08,
     616,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0addDataFromFile()\0"
    "clearCSVList()\0axis,part\0"
    "axisLabelClick(QCPAxis*,QCPAxis::SelectablePart)\0"
    "legend,item\0legendClick(QCPLegend*,QCPAbstractLegendItem*)\0"
    "on_actionExport_triggered()\0"
    "on_plotUebertragen_clicked()\0"
    "on_plotUebertragenFlach_clicked()\0"
    "on_action_Hilfe_triggered()\0"
    "on_toolButtonBold_clicked()\0"
    "on_toolButtonUnterstrichen_clicked()\0"
    "on_toolButtonItalic_clicked()\0arg1\0"
    "on_comboSize_currentIndexChanged(QString)\0"
    "on_fontComboBox_currentIndexChanged(QString)\0"
    "on_pushFoto_clicked()\0"
    "on_actionSpeichern_triggered()\0"
    "on_actionOffnen_triggered()\0"
    "on_checkBoxLegende_toggled()\0"
    "on_spinBoxIdealHum_valueChanged(int)\0"
    "on_textEdit_cursorPositionChanged()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->addDataFromFile(); break;
        case 1: _t->clearCSVList(); break;
        case 2: _t->axisLabelClick((*reinterpret_cast< QCPAxis*(*)>(_a[1])),(*reinterpret_cast< QCPAxis::SelectablePart(*)>(_a[2]))); break;
        case 3: _t->legendClick((*reinterpret_cast< QCPLegend*(*)>(_a[1])),(*reinterpret_cast< QCPAbstractLegendItem*(*)>(_a[2]))); break;
        case 4: _t->on_actionExport_triggered(); break;
        case 5: _t->on_plotUebertragen_clicked(); break;
        case 6: _t->on_plotUebertragenFlach_clicked(); break;
        case 7: _t->on_action_Hilfe_triggered(); break;
        case 8: _t->on_toolButtonBold_clicked(); break;
        case 9: _t->on_toolButtonUnterstrichen_clicked(); break;
        case 10: _t->on_toolButtonItalic_clicked(); break;
        case 11: _t->on_comboSize_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 12: _t->on_fontComboBox_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 13: _t->on_pushFoto_clicked(); break;
        case 14: _t->on_actionSpeichern_triggered(); break;
        case 15: _t->on_actionOffnen_triggered(); break;
        case 16: _t->on_checkBoxLegende_toggled(); break;
        case 17: _t->on_spinBoxIdealHum_valueChanged((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 18: _t->on_textEdit_cursorPositionChanged(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
