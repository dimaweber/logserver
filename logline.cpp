#include "logline.h"
#include <QLocale>
#include <iomanip>

std::ostream& operator<<(std::ostream& stream, const LogLine& line)
{
    auto output = [&stream](const QString& name, const QString& value)
    {
#if MULTILINE_OUTPUT
        stream <<  '\t' << std::setw(10) << std::left << qPrintable(name)  << ':' << qPrintable(value) << std::endl;
#else
        Q_UNUSED(name);
        stream <<  std::setw(10) <<  std::left << qPrintable(value);
#endif
    };

    line.forEachField([&output](LogLine::Fields field, const QVariant& value)
    {
        output (LogLine::fieldName(field), value.toString());
    });

    return stream;
}

LogLine::LogLine(const QByteArray &data, QLocale *logLocale)
{
    QString str = QString::fromUtf8(data.constData());
    static const QRegExp lineRegExp("<([0-9]{1,4})>([a-zA-Z]{3}\\s+[0-9]{1,2}\\s+[0-9]{1,2}:[0-9]{1,2}:[0-9]{1,2})\\s*([a-zA-z]*)\\s*([a-zA-Z_]*):(.*)");
    static const QRegExp extendedLogging("\\[(.*):([0-9]*)\\s+(.*)\\] (.*)");

    if (lineRegExp.indexIn(str) != -1)
    {
        pri = lineRegExp.cap(1).toInt();
        if (logLocale)
            logDateTime = logLocale->toDateTime(lineRegExp.cap(2).simplified(), "MMM d HH:mm:ss");
        else
            logDateTime = QDateTime::fromString(lineRegExp.cap(2).simplified(), "MMM d HH:mm:ss");
        hostName = lineRegExp.cap(3);
        processName = lineRegExp.cap(4);
        if (processName.isEmpty())
        {
            processName = hostName;
            hostName = "localhost";
        }
        message = lineRegExp.cap(5);

        if (extendedLogging.indexIn(message) != -1)
        {
            exLog = true;
            fileName = extendedLogging.cap(1);
            lineNum = extendedLogging.cap(2).toInt();
            functionName = extendedLogging.cap(3);
            message = extendedLogging.cap(4);
        }
        else
        {
            exLog = false;
            fileName.clear();
            lineNum = 0;
            functionName.clear();
        }
    }
    serverDateTime = QDateTime::currentDateTime();
}

bool LogLine::isFilterable(LogLine::Fields field)
{
    switch (field)
    {
    case LogLine::Priority:
    case LogLine::ServerTimestamp:
    case LogLine::LogTimestamp:
    case LogLine::HostName:
    case LogLine::ProcessName:
        return true;
    default:
        return false;
    }
}

bool LogLine::isMinMaxFilter(LogLine::Fields field)
{
    switch (field)
    {
        case ServerTimestamp:
        case LogTimestamp:
            return true;
        default:
            return false;
    }
}

bool LogLine::isSetFilter(LogLine::Fields field)
{
    switch (field)
    {
        case Priority:
        case HostName:
        case ProcessName:
            return true;
        default:
            return false;
    }
}

QVariant LogLine::fieldValue(LogLine::Fields field) const
{
    switch(field)
    {
        case LogLine::Priority: return priority();
        case LogLine::ServerTimestamp: return serverDateTime;
        case LogLine::LogTimestamp: return logDateTime;
        case LogLine::ProcessName: return processName;
        case LogLine::HostName: return hostName;
        case LogLine::FileName: return fileName;
        case LogLine::LineNum: return lineNum;
        case LogLine::FunctionName: return functionName;
        case LogLine::Message: return message;
        default: return QVariant();
    }
}

void LogLine::forEachField(std::function<void (LogLine::Fields, const QVariant &)> func) const
{
    for (int i=0; i<LogLine::LAST; i++)
    {
        LogLine::Fields field = static_cast<LogLine::Fields>(i);
        QVariant value = fieldValue(field);

        func(field, value);
    }
}

QString LogLine::fieldName(LogLine::Fields field)
{
    switch(field)
    {
        case LogLine::Priority:     return "Priority";
        case LogLine::ServerTimestamp:
        case LogLine::LogTimestamp: return "Timestamp";
        case LogLine::ProcessName:  return "process";
        case LogLine::HostName:     return "host";
        case LogLine::FileName:     return "file";
        case LogLine::LineNum:      return "line";
        case LogLine::FunctionName: return "function";
        case LogLine::Message:      return "message";
        default:                    return "<none>";
    }
}

QString LogLine::priority() const
{
    switch (pri)
    {
    case 14: return "info";
    case 15: return "note";
    case 13: return "debug";
    default: return "other";
    }
}
