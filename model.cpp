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
    return 8;
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
                 case 0: return ptr->id;
                 case 1: return ptr->serverDateTime;
                 case 2: return ptr->processName;
                 case 3: return ptr->hostName;
                 case 4: return ptr->fileName;
                 case 5: return ptr->lineNum;
                 case 6: return ptr->functionName;
                 case 7: return ptr->message;
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
                case 0: return "Priority";
                case 1: return "Timestamp";
                case 2: return "process";
                case 3: return "host";
                case 4: return "file";
                case 5: return "line";
                case 6: return "function";
                case 7: return "message";
            }
        }
        else
            return section+1;
    }

    return QVariant();
}

