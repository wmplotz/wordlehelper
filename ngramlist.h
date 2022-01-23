#ifndef NGRAMLIST_H
#define NGRAMLIST_H

#include <QString>
#include <QHash>

class NGramList
{
    const int m_Width;
    const QHash<QString, double> m_Frequency;
    double m_MinValue;

    QHash<QString, double> read_data(int width, QString file_name);

public:
    NGramList(int width, QString file_name);

    double word_relevance(QString word, QStringList* used_items = Q_NULLPTR) const;
    double min_relevance() const;
};

#endif // NGRAMLIST_H
