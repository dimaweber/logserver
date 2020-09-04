#ifndef LOGLINE_H
#define LOGLINE_H

#include <QString>
#include <QDateTime>
#include <QVariant>

#include <iostream>

struct LogLine
{
    enum Fields {ServerTimestamp, Priority, LogTimestamp, HostName, ProcessName, FileName, LineNum, FunctionName, Message, LAST};
    uint priority;
    QDateTime logDateTime;
    QDateTime serverDateTime;
    QString hostName;
    QString processName;
    QString fileName;
    quint32 lineNum;
    QString functionName;
    bool exLog;
    QString message;

    static bool    isFilterable(Fields field);
    static QString fieldName(Fields field);

    QVariant fieldValue(Fields field) const;

};

std::ostream& operator<< (std::ostream& stream, const LogLine& line);
#endif // LOGLINE_H
