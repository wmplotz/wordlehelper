#include "ngramlist.h"

#include <QFile>
#include <QList>
#include <QPair>

#include <QTextStream>
#include <QDebug>

#include <math.h>

QHash<QString, double> NGramList::read_data(int width, QString file_name)
{
    QHash<QString, double> result;
    QList<QPair<QString, double>> data;

    double max_value=1.0;
    m_MinValue = 0.0;

    QFile file(file_name);

    if (!file.open(QIODevice::ReadOnly)) {
        exit(1);
    } else {
        QTextStream in(&file);
        QRegExp splitter("[\r\n\t ]+");
        while (!in.atEnd())
        {
           QString a_line = in.readLine().trimmed();
           QStringList items = a_line.split(splitter, QString::SkipEmptyParts);

           if (width == items[0].length()) {
               double value = items[1].toDouble();
               max_value = max_value >= value ? max_value : value;
               data.append(QPair<QString, double>(items[0],value));
           } else {
               qWarning() << "Wrong word in: " << file_name << ": " << items[0];
           }
        }
        file.close();
    }

    while (!data.empty()) {
        QPair<QString, double> item(data.takeFirst());
        const double value = log(item.second / max_value);
        m_MinValue = m_MinValue < value ? m_MinValue : value;
        result[item.first] = value;
        //qDebug() << item.first << value;
    }

    //qDebug() << m_MinValue;
    return result;
}

NGramList::NGramList(int width, QString file_name)
    : m_Width(width),
      m_Frequency(read_data(width, file_name))
{
}

double NGramList::min_relevance() const
{
    return m_MinValue * m_Width;
}

double NGramList::word_relevance(QString word, QStringList* used_items) const
{
    const int max_offset = word.length() - m_Width;

    if (max_offset < 0) {
        return static_cast<double>(NAN);
    } else {
        double result=0.0;
        for(int offset=0; offset <= max_offset; offset++) {
            QString item(word.mid(offset, m_Width));
            result += m_Frequency.value(item);
            if (used_items && !used_items->contains(item)) {
                used_items->append(item);
            }
        }
        return result;
    }
}

