#include "logline.h"
#include <iomanip>

std::ostream& operator<<(std::ostream& stream, const LogLine& line)
{
    auto output = [&stream](const char*name, const QString& value)
    {
#if MULTILINE_OUTPUT
        stream <<  '\t' << std::setw(10) << std::left << name  << ':' << qPrintable(value) << std::endl;
#else
        Q_UNUSED(name);
        stream <<  std::setw(10) <<  std::left << qPrintable(value);
#endif
    };
    output("id", QString::number(line.priority));
    output("date", line.serverDateTime.toString("hh:mm:ss.zzz"));
    output("host", line.hostName);
    output("process",  line.processName);
    //if (line.exLog)
    {
        output("filename", line.fileName);
        output("linenum", QString::number(line.lineNum));
        output("function", line.functionName);
    }
    output("message", line.message);
    return stream;
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

QVariant LogLine::fieldValue(LogLine::Fields field) const
{
    switch(field)
    {
        case LogLine::Priority: return priority;
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

QString LogLine::fieldName(LogLine::Fields field)
{
    switch(field)
    {
        case LogLine::Priority: return "Priority";
        case LogLine::ServerTimestamp:
        case LogLine::LogTimestamp:
            return "Timestamp";
        case LogLine::ProcessName: return "process";
        case LogLine::HostName: return "host";
        case LogLine::FileName: return "file";
        case LogLine::LineNum: return "line";
        case LogLine::FunctionName: return "function";
        case LogLine::Message: return "message";
        default: return "<none>";
    }
}
