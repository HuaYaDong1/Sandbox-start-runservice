QT       += core gui
QT  +=dbus
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    bwrap_run_thread.cpp \
    main.cpp \
    service.cpp \
    service_2.cpp

HEADERS += \
    bwrap_run_thread.h \
    service.h \
    service_2.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
