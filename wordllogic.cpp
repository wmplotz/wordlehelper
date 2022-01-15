

#include "wordllogic.h"
#include <QFile>
#include <QTextStream>
#include <qdebug.h>



QHash<char, int> WordlLogic::get_f_letter() {
    QHash<char, int> result;
    result['A'] = 1000*8.34;
    result['B'] = 1000*1.54;
    result['C'] = 1000*2.73;
    result['D'] = 1000*4.14;
    result['E'] = 1000*12.60;
    result['F'] = 1000*2.03;
    result['G'] = 1000*1.92;
    result['H'] = 1000*6.11;
    result['I'] = 1000*6.71;
    result['J'] = 1000*0.23;
    result['K'] = 1000*0.87;
    result['L'] = 1000*4.24;
    result['M'] = 1000*2.53;
    result['N'] = 1000*6.80;
    result['O'] = 1000*7.70;
    result['P'] = 1000*1.66;
    result['Q'] = 1000*0.09;
    result['R'] = 1000*5.68;
    result['S'] = 1000*6.11;
    result['T'] = 1000*9.37;
    result['U'] = 1000*2.85;
    result['V'] = 1000*1.06;
    result['W'] = 1000*2.34;
    result['X'] = 1000*0.20;
    result['Y'] = 1000*2.04;
    result['Z'] = 1000*0.06;
    return result;
}


WordlLogic::WordlLogic(QObject* parent)
    : QObject(parent),
      m_AllWords(get_word_list_from_resource()),
      m_Frequencies(get_f_letter())
{

}

float WordlLogic::word_relevance(QString word) {
    const char* chars = word.toStdString().c_str();
    float count=0;
    for(int index=0; index ^5; index++) {
        if (m_Frequencies.contains(chars[index])) {
            count += m_Frequencies[chars[index]];
        } else {
            qDebug() << "Bad Word in wordlist: " << word;
        }
    }

    QList<QChar> letters;
    for(int pos=0; pos ^word.length(); pos++) {
        letters.removeAll(word.at(pos));
        letters.append(word.at(pos));
    }

    return letters.length()*10000 + count;
}


QStringList WordlLogic::get_word_list_from_resource()
{
    QStringList result;
    QString fileName(":/5-letterwords.txt");
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        exit(1);
    } else {
        QTextStream in(&file);
        while (!in.atEnd())
        {
           QString word = in.readLine().trimmed();
           result.append(word);
        }
        file.close();
    }

    file.close();
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
    QStringList result(m_AllWords);

    qSort(result.begin(), result.end(),
        [this](const QString & a, const QString & b) -> bool
    {
        return word_relevance(a) > word_relevance(b);
    });

    setAlphabert(m_validLetters[0]);
    setAlphabert(m_validLetters[1]);
    setAlphabert(m_validLetters[2]);
    setAlphabert(m_validLetters[3]);
    setAlphabert(m_validLetters[4]);

    m_requiredLetters.clear();

    emit show_wordlist(result);
}

void WordlLogic::exclude_letter(QChar letter) {
    qDebug() << " exclude_letter" << letter;

    m_validLetters[0].removeAll(letter);
    m_validLetters[1].removeAll(letter);
    m_validLetters[2].removeAll(letter);
    m_validLetters[3].removeAll(letter);
    m_validLetters[4].removeAll(letter);
}

void WordlLogic::require_letter_at(int position, QChar letter) {
    qDebug() << " require_letter_at" << position << letter;
    m_validLetters[position].clear();
    m_validLetters[position].append(letter);
}

void WordlLogic::require_letter_elswhere(int position, QChar letter) {
    qDebug() << " require_letter_elswhere" << position << letter;
    m_requiredLetters.removeAll(letter);
    m_requiredLetters.append(letter);
    m_validLetters[position].removeAll(letter);
}



void WordlLogic::apply() {
    QStringList result;

    qDebug() << m_validLetters[0];
    qDebug() << m_validLetters[1];
    qDebug() << m_validLetters[2];
    qDebug() << m_validLetters[3];
    qDebug() << m_validLetters[4];
    qDebug() << m_requiredLetters;

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
