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

    pLine->forEachField([this](LogLine::Fields field, const QVariant& value)
    {
        if (LogLine::isSetFilter(field))
        {
            if (!possibleValues[field].contains(value.toString()))
            {
                possibleValues[field].insert(value.toString());
                emit newFilterValue(field, value.toString());
            }
        }

        if (LogLine::isMinMaxFilter(field))
        {
            timestampRanges[field].first = timestampRanges[field].first.isValid()?
                        qMin(timestampRanges[field].first, value.toDateTime())
                      : value.toDateTime();
            timestampRanges[field].second = qMax(timestampRanges[field].first, value.toDateTime());
        }
    });

    endInsertRows();
}

Model::~Model()
{
    foreach(LogLine* p, lines)
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
            LogLine* ptr = lines[index.row()];
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

