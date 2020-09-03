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
    output("id", QString::number(line.id));
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
