#include "model.h"

Model::Model(QObject* parent)
    :QAbstractTableModel(parent)
{
}

void Model::onNewLine(LogLine* pLine)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    std::cout << *pLine << std::endl;
    lines.append(pLine);
    endInsertRows();
}

Model::~Model()
{
    for(LogLine* p: lines)
        delete p;
}


int Model::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return lines.count();
}

int Model::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return LogLine::LAST;
}

QVariant Model::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role)
    {
        case Qt::DisplayRole: 
        {
            LogLine* ptr = lines.at(index.row());
            switch(index.column())
            {
                 case LogLine::Priority: return ptr->id;
                 case LogLine::ServerTimestamp: return ptr->serverDateTime;
                 case LogLine::LogTimestamp: return ptr->logDateTime;
                 case LogLine::ProcessName: return ptr->processName;
                 case LogLine::HostName: return ptr->hostName;
                 case LogLine::FileName: return ptr->fileName;
                 case LogLine::LineNum: return ptr->lineNum;
                 case LogLine::FunctionName: return ptr->functionName;
                 case LogLine::Message: return ptr->message;
            }
        }
        default:     
            return QVariant();
    }
}

QVariant Model::headerData(int section, Qt::Orientation orientation,  int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch(section)
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
            }
        }
        else
            return section+1;
    }

    return QVariant();
}

