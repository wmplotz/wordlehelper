#include "teststrategy.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QtGlobal>

TestStrategy::TestStrategy(QObject* parent) :
    QObject(parent),
    m_Logic(new WordlLogic(this))
{
    for(int index=0; index < MAX_TRIES+1; index++) {
        success_counters[index] = 0;
    }

    connect(m_Logic, &WordlLogic::show_wordlist, [this](QStringList wordlist) {
       this->m_Suggestions = wordlist;
    });
}


QString TestStrategy::matched_letters(QString word, QString guess) {
    QString result;
    for(int idx=0; idx < word.length(); idx++) {
        if (word.at(idx) != guess.at(idx)) {
            result.append('.');
            if (word.indexOf(guess.at(idx)) != -1) {
                m_Logic->require_letter_elswhere(idx, guess.at(idx));
            }
        } else {
            result.append(word.at(idx));
            m_Logic->require_letter_at(idx, word.at(idx));
        }
    }
    return result;
}

QString TestStrategy::absent_letters(QString word, QString guess, QList<QChar>& missing_letters) {
    for(int idx=0; idx < word.length(); idx++) {
        if ((word.indexOf(guess.at(idx)) == -1) && !missing_letters.contains(guess.at(idx))) {
            missing_letters.append(guess.at(idx));
            m_Logic->exclude_letter(guess.at(idx));
        }
    }

    QList<QChar>& dummy(missing_letters);
    QString result;
    while (!dummy.empty()) {
        result.append(dummy.takeFirst());
    }
    return result;
}


void TestStrategy::test_at(QString word) {
    m_Suggestions.clear();
    m_Logic->reset();

    QList<QChar> missing_letter_lists;

    int count=0;
    while (count < MAX_TRIES) {
        QString guess = m_Suggestions.first();
        QString missing = absent_letters(word, guess, missing_letter_lists);
        QString matched = matched_letters(word, guess);

        qDebug() << (count+1) << word << guess << matched << missing;

        if (word.compare(guess)==0) {
            qDebug() << "Solved in guess " << (count+1);
            success_counters[count]++;
            return;
        }
        count++;
        m_Logic->apply();
    }
    success_counters[count]++;
    qDebug() << "NOT Solved in " << MAX_TRIES <<  "guesses";
}

void TestStrategy::run(QString file_name) {
    QFile file(file_name);

    if (!file.open(QIODevice::ReadOnly)) {
        exit(1);
    } else {
        QTextStream in(&file);
        QRegExp splitter("[\r\n\t ]+");
        while (!in.atEnd())
        {
            QString a_word = in.readLine().trimmed().right(5);
            qDebug() << "Searching" << a_word << ":";

            test_at(a_word);
        }
        file.close();
    }

    int sum=0;
    int sum_x_n=0;
    for(int index=0; index < MAX_TRIES+1; index++) {
        qDebug() << (index+1) << success_counters[index];
        sum += success_counters[index];
        sum_x_n += success_counters[index] * (index+1);
    }

     qDebug() << "Average:" << static_cast<double>(sum_x_n)/sum << " failed: " << success_counters[MAX_TRIES];
}
