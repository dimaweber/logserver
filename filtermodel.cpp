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

bool FilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    bool retVal = true;
    foreach (LogLine::Fields field, filterSet.keys())
    {
        QModelIndex index = sourceModel()->index(source_row, field, source_parent);
        const QVariant& var = filterSet[field];
        QVariant::Type type = var.type();
        switch (type)
        {
            case QVariant::RegExp:
                retVal &= var.toRegExp().indexIn(sourceModel()->data(index).toString()) != -1;
                break;
            case QVariant::String:
            case QVariant::Int:
                retVal &= sourceModel()->data(index).toString().contains(var.toString());
                break;
            case QVariant::StringList:
                {
                    bool inList = false;
                    foreach(const QString& varElem, var.toStringList())
                    {
                        inList |= sourceModel()->data(index).toString().contains(varElem);
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
