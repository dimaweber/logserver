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

    possibleValues[LogLine::Priority].insert(QString::number(pLine->priority));
    possibleValues[LogLine::ProcessName].insert(pLine->processName);
    possibleValues[LogLine::HostName].insert(pLine->hostName);

    timestampRanges[LogLine::ServerTimestamp].first = qMin(timestampRanges[LogLine::ServerTimestamp].first, pLine->serverDateTime);
    timestampRanges[LogLine::ServerTimestamp].second = qMax(timestampRanges[LogLine::ServerTimestamp].first, pLine->serverDateTime);
    timestampRanges[LogLine::LogTimestamp].first = qMin(timestampRanges[LogLine::LogTimestamp].first, pLine->logDateTime);
    timestampRanges[LogLine::LogTimestamp].second = qMax(timestampRanges[LogLine::LogTimestamp].first, pLine->logDateTime);

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
            return ptr->fieldValue(static_cast<LogLine::Fields>(index.column()));
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
            return LogLine::fieldName(static_cast<LogLine::Fields>(section));
        }
        else
            return section+1;
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

