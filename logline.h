#ifndef LOGLINE_H
#define LOGLINE_H

#include <QString>
#include <QDateTime>
#include <iostream>

struct LogLine
{
    enum Fields {ServerTimestamp, Priority, LogTimestamp, HostName, ProcessName, FileName, LineNum, FunctionName, Message, LAST};
    quint16 id;
    QDateTime logDateTime;
    QDateTime serverDateTime;
    QString hostName;
    QString processName;
    QString fileName;
    quint32 lineNum;
    QString functionName;
    bool exLog;
    QString message;
};

std::ostream& operator<< (std::ostream& stream, const LogLine& line);
#endif // LOGLINE_H
