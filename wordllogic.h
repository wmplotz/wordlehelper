#ifndef WORDLLOGIC_H
#define WORDLLOGIC_H

#include <qobject.h>
#include <qlist.h>
#include <QStringList>
#include <QHash>

#include "ngramlist.h"

class WordlLogic : public QObject
{
    Q_OBJECT


    const NGramList m_LetterStatisitc;
    const NGramList m_2GramStatisitc;
    const NGramList m_3GramStatisitc;

    double word_relevance(QString word);

    QList<QChar> m_validLetters[5];
    QList<QChar> m_requiredLetters;
    bool accept_word(QString word);

    int m_ApplyCount;
    const QStringList m_AllWords;
    QStringList get_word_list_from_resource(QString file_name);

public:
    WordlLogic(QObject* parent);

    void reset();

    void exclude_letter(QChar letter);
    void require_letter_at(int position, QChar letter);
    void require_letter_elswhere(int position, QChar letter);

    void apply();

signals:
    void show_wordlist(const QStringList);

};

#endif // WORDLLOGIC_H
