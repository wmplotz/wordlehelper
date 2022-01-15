#ifndef WORDLLOGIC_H
#define WORDLLOGIC_H

#include <qobject.h>
#include <qlist.h>
#include <QStringList>
#include <QHash>

class WordlLogic : public QObject
{
    Q_OBJECT

    const QStringList m_AllWords;
    const QHash<char, int> m_Frequencies;

    QStringList get_word_list_from_resource();
    QHash<char, int> get_f_letter();

    float word_relevance(QString word);

    QList<QChar> m_validLetters[5];
    QList<QChar> m_requiredLetters;
    bool accept_word(QString word);


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
