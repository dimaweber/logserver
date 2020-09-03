#ifndef FILTERMODEL_H
#define FILTERMODEL_H

#include "logline.h"
#include <QSortFilterProxyModel>
#include <QMap>

class FilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
    QMap<LogLine::Fields, QVariant> filterSet;
public:
    FilterModel(QObject *parent = nullptr);

public slots:
    void addFilter(LogLine::Fields field, QVariant filterValue);
    void resetFilter();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;
};

#endif // FILTERMODEL_H
