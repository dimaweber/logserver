#include "filtermodel.h"

FilterModel::FilterModel(QObject* parent)
    :QSortFilterProxyModel(parent)
{

}

void FilterModel::addFilter(LogLine::Fields field, QVariant filterValue)
{
    filterSet[field] = filterValue;
}

void FilterModel::resetFilter()
{
    filterSet.clear();
}

bool FilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    bool retVal = true;
    for (LogLine::Fields f: filterSet.keys())
    {
        QModelIndex index = sourceModel()->index(source_row, f, source_parent);

        retVal &= sourceModel()->data(index).toString().contains(filterSet[f].toString());
        if (!retVal)
            break;
    }

    return retVal;
}
