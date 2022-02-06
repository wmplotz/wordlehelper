

#include "wordllogic.h"
#include <QFile>
#include <QTextStream>
#include <qdebug.h>

#include <math.h>


#define FILENAME_1 ":/bigrams.txt"
//#define FILENAME_2 ":/all_words.txt"
#define FILENAME_2 ":/5-letterwords.txt"

WordlLogic::WordlLogic(QObject* parent)
    : QObject(parent),
      m_LetterStatisitc(1, ":/english_monograms.txt"),
      m_2GramStatisitc(2, ":/english_bigrams.txt"),
      m_3GramStatisitc(3, ":/english_trigrams.txt"),
      m_ApplyCount(0),
      m_AllWords(get_word_list_from_resource(":/wordle_list.txt"))
//    m_AllWords(get_word_list_from_resource(":/5-letterwords.txt"))
{
    //qDebug() << m_LetterStatisitc.min_relevance();
    //qDebug() << m_2GramStatisitc.min_relevance();
    //qDebug() << m_3GramStatisitc.min_relevance();
}

double WordlLogic::word_relevance(QString word) {

    const NGramList& ngrams(m_ApplyCount > 3 ? m_2GramStatisitc : m_3GramStatisitc);

    QStringList used_letters;
    double letter_value = m_LetterStatisitc.word_relevance(word, &used_letters);

    double result = m_ApplyCount < 3 ? used_letters.length() : 1.0;

    // result = result * 1000 + letter_value;
    result = (result*fabs(ngrams.min_relevance())*1000) + ngrams.word_relevance(word) * 1000;
    return result;
}

QStringList WordlLogic::get_word_list_from_resource(QString file_name)
{
    QStringList result;
    QFile file(file_name);

    if (!file.open(QIODevice::ReadOnly)) {
        exit(1);
    } else {
        QTextStream in(&file);
        while (!in.atEnd())
        {
           QString word = in.readLine().trimmed().toUpper();
           if (word.length()==5) {
               result.append(word);
           }
        }
        file.close();
    }

    file.close();

    qSort(result.begin(), result.end(),
        [this](const QString & a, const QString & b) -> bool
    {
        return word_relevance(a) > word_relevance(b);
    });

    return result;
}

static void setAlphabert(QList<QChar>& data) {
    data.clear();
    data << 'A' << 'B' << 'C' << 'D' << 'E' << 'F' << 'G' << 'H'
         << 'I' << 'J' << 'K' << 'L' << 'M' << 'N' << 'O' << 'P'
         << 'Q' << 'R' << 'S' << 'T' << 'U' << 'V' << 'W' << 'X'
         << 'Y' << 'Z';
}

void WordlLogic::reset() {
    m_ApplyCount = 0;
    QStringList result(m_AllWords);

    setAlphabert(m_validLetters[0]);
    setAlphabert(m_validLetters[1]);
    setAlphabert(m_validLetters[2]);
    setAlphabert(m_validLetters[3]);
    setAlphabert(m_validLetters[4]);

    m_requiredLetters.clear();

    m_ApplyCount = 0;

    emit show_wordlist(result);
}

void WordlLogic::exclude_letter(QChar letter) {
    //qDebug() << " exclude_letter" << letter;

    m_validLetters[0].removeAll(letter);
    m_validLetters[1].removeAll(letter);
    m_validLetters[2].removeAll(letter);
    m_validLetters[3].removeAll(letter);
    m_validLetters[4].removeAll(letter);
}

void WordlLogic::require_letter_at(int position, QChar letter) {
    //qDebug() << " require_letter_at" << position << letter;
    m_validLetters[position].clear();
    m_validLetters[position].append(letter);
}

void WordlLogic::require_letter_elswhere(int position, QChar letter) {
    //qDebug() << " require_letter_elswhere" << position << letter;
    m_requiredLetters.removeAll(letter);
    m_requiredLetters.append(letter);
    m_validLetters[position].removeAll(letter);
}



void WordlLogic::apply() {
    QStringList result;
#ifdef LOG_VALID_LETTERS
    qDebug() << m_validLetters[0];
    qDebug() << m_validLetters[1];
    qDebug() << m_validLetters[2];
    qDebug() << m_validLetters[3];
    qDebug() << m_validLetters[4];
    qDebug() << m_requiredLetters;
#endif
    foreach(QString word, m_AllWords) {
        if (accept_word(word)) {
            result.append(word);
        }
    }

    qSort(result.begin(), result.end(),
        [this](const QString & a, const QString & b) -> bool
    {
        return word_relevance(a) > word_relevance(b);
    });

    m_ApplyCount++;

    emit show_wordlist(result);
}


bool WordlLogic::accept_word(QString word) {

    for(int pos=0; pos < 5; pos++) {
        QChar letter(word.at(pos));
        if (!m_validLetters[pos].contains(letter)) {
            return false;
        }
    }

    for(int pos=0; pos < m_requiredLetters.length(); pos++) {
        QChar letter(m_requiredLetters.at(pos));
        if (!word.contains(letter)) {
            return false;
        }
    }

    return true;
}
