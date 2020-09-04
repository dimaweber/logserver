#ifndef FILTERMODEL_H
#define FILTERMODEL_H

#include "logline.h"
#include <QSortFilterProxyModel>
#include <QMap>

class FilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
    QMap<LogLine::Fields, QVariant> filterSet;
    QVector<int> hiddenRows;
    QStringList hiddenMessages;
public:
    FilterModel(QObject *parent = nullptr);

public slots:
    void addFilter(LogLine::Fields field, QVariant filterValue);
    void removeFilter(LogLine::Fields field);
    void resetFilter();

    void hideRow(QModelIndex index);
    void hideRowsWithText(const QString& text);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;
};

#endif // FILTERMODEL_H
