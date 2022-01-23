#include "wordlehelpermainwindow.h"

#ifdef RUN_TEST_STRATEGY
#include "teststrategy.h"
#endif

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WordleHelperMainWindow w;

#ifdef RUN_TEST_STRATEGY
    TestStrategy().run("result_list.txt");
    return 0;
#else
    w.show();
    return a.exec();
#endif

}

