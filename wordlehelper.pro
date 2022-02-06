QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += xRUN_TEST_STRATEGY

SOURCES += \
    main.cpp \
    wordlehelpermainwindow.cpp \
    wordllogic.cpp \
    ngramlist.cpp \
    teststrategy.cpp

HEADERS += \
    wordlehelpermainwindow.h \
    wordllogic.h \
    ngramlist.h \
    teststrategy.h

FORMS += \
    wordlehelpermainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    result_list.txt
