#ifndef LOGLINE_H
#define LOGLINE_H

#include <QString>
#include <QDateTime>
#include <QVariant>

#include <iostream>

class QLocale;
struct LogLine
{
    enum Fields {ServerTimestamp, Priority, LogTimestamp, HostName, ProcessName, FileName, LineNum, FunctionName, Message, LAST};
    uint pri;
    QDateTime logDateTime;
    QDateTime serverDateTime;
    QString hostName;
    QString processName;
    QString fileName;
    quint32 lineNum;
    QString functionName;
    bool exLog;
    QString message;

    LogLine(const QByteArray& data, QLocale* logLocale = nullptr);

    static bool    isFilterable(Fields field);
    static bool    isMinMaxFilter(Fields field);
    static bool    isSetFilter(Fields field);

    static QString fieldName(Fields field);
    QString priority () const;

    QVariant fieldValue(Fields field) const;

    void forEachField( std::function<void(LogLine::Fields, const QVariant&)>) const;

};

std::ostream& operator<< (std::ostream& stream, const LogLine& line);
#endif // LOGLINE_H
