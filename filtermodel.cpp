#include "filtermodel.h"
#include <QDebug>

FilterModel::FilterModel(QObject* parent)
    :QSortFilterProxyModel(parent)
{

}

void FilterModel::addFilter(LogLine::Fields field, QVariant filterValue)
{
    if (LogLine::isFilterable(field))
        filterSet[field] = filterValue;
    else
        qWarning() << "field " << field << " cannot be filtered";
    invalidateFilter();
}

void FilterModel::removeFilter(LogLine::Fields field)
{
    filterSet.remove(field);
    invalidateFilter();
}

void FilterModel::resetFilter()
{
    filterSet.clear();
    invalidateFilter();
}

void FilterModel::hideRow(QModelIndex index)
{
    hiddenRows.append(index.row());
    invalidateFilter();
}

void FilterModel::hideRowsWithText(const QString &text)
{
    hiddenMessages.append(text);
    invalidateFilter();
}

bool FilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    bool retVal = true;

    if (!hiddenRows.isEmpty() && hiddenRows.contains(source_row))
        return false;

    if (!hiddenMessages.isEmpty())
    {
        QModelIndex messageIndex = sourceModel()->index(source_row, LogLine::Message, source_parent);
        if (hiddenMessages.contains(sourceModel()->data(messageIndex).toString()))
            return false;
    }

    foreach (LogLine::Fields field, filterSet.keys())
    {
        QModelIndex index = sourceModel()->index(source_row, field, source_parent);
        const QVariant& var = filterSet[field];
        QVariant::Type type = var.type();
        const QString& fieldValue = sourceModel()->data(index).toString();
        switch (type)
        {
            case QVariant::RegExp:
                retVal &= var.toRegExp().indexIn(fieldValue) != -1;
                break;
            case QVariant::String:
            case QVariant::Int:
                retVal &= fieldValue.contains(var.toString());
                break;
            case QVariant::StringList:
                {
                    bool inList = false;
                    foreach(const QString& varElem, var.toStringList())
                    {
                        inList |= fieldValue.contains(varElem);
                        if (inList)
                            break;
                    }
                    retVal &= inList;
                }
                break;
            default:
                retVal = false;
        }

        if (!retVal)
            break;
    }

    return retVal;
}
