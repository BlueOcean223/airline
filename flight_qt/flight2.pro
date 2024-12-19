QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    allSoure.cpp \
    bookpage.cpp \
    boughtcard.cpp \
    flightcard.cpp \
    main.cpp \
    mainwindow.cpp \
    myswapbutton.cpp \
    myticketpage.cpp \
    rebookpage.cpp \
    searchpage.cpp

HEADERS += \
    allSoure.h \
    bookpage.h \
    boughtcard.h \
    flightcard.h \
    mainwindow.h \
    myswapbutton.h \
    myticketpage.h \
    rebookpage.h \
    searchpage.h

FORMS += \
    boughtcard.ui \
    flightcard.ui \
    mainwindow.ui \
    searchpage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc
