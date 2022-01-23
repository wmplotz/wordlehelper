#ifndef TESTSTRATEGY_H
#define TESTSTRATEGY_H

#include <QString>
#include <QObject>
#include <QStringList>
#include "wordllogic.h"

class TestStrategy : public QObject
{
    Q_OBJECT


    static const int MAX_TRIES = 6;
    int success_counters[MAX_TRIES+1];

    WordlLogic* m_Logic;
    QStringList m_Suggestions;

    QString matched_letters(QString word, QString guess);
    QString absent_letters(QString word, QString guess, QList<QChar>& missing_letters);

    virtual void test_at(QString word);

public:
    TestStrategy(QObject* parent = Q_NULLPTR);
    virtual ~TestStrategy() {}

    virtual void run(QString fileName);

};

#endif // TESTSTRATEGY_H
