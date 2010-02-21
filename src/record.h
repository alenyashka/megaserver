//TODO #pragma one
#ifndef RECORD_H
#define RECORD_H

#include <QtCore>

class Record
{
public:
    Record(const QString&);
    Record(const QString&, const QString&, const bool&,
           const QVariant::Type&, const QVariant&);
    QString getTitle() const;
    void setTitle(const QString&);
    QString getComment() const;
    void setComment(const QString&);
    bool isReadOnly() const;
    void setReadOnly(const bool&);
    QVariant getValue() const;
    void setValue (const QVariant&);
    QVariant::Type getType () const;
    bool operator == (const Record&) const;

private:
    QString title;
    QString comment;
    bool readOnly;
    QVariant::Type type;
    QVariant value;
};

#endif // RECORD_H
