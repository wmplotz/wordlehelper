#include "wordlehelpermainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WordleHelperMainWindow w;
    w.show();
    return a.exec();
}

