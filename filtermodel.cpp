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
        qDebug() << "field " << field << " cannot be filtered";
}

void FilterModel::resetFilter()
{
    filterSet.clear();
}

bool FilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    bool retVal = true;
    for (LogLine::Fields field: filterSet.keys())
    {
        QModelIndex index = sourceModel()->index(source_row, field, source_parent);

        retVal &= sourceModel()->data(index).toString().contains(filterSet[field].toString());
        if (!retVal)
            break;
    }

    return retVal;
}
