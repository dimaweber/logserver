#include "gui.h"
#include "filtermodel.h"
#include "stringfilterwidget.h"
#include "model.h"

#include <QTableView>
#include <QHeaderView>
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>

Gui::Gui(Model* pModel, QWidget *parent)
    : QMainWindow(parent), pTableView(nullptr), pModel(pModel), pProxyModel(new FilterModel(this))
{
    pTableView = new QTableView(this);
    pHeader = new HorizontalHeader(this);
    setMinimumSize(1200, 800);
    pTableView->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    pProxyModel->setSourceModel(pModel);
    pTableView->setModel(pProxyModel);
    pTableView->setHorizontalHeader(pHeader);
    pHeader->setStretchLastSection(true);
    pHeader->setSectionResizeMode(QHeaderView::ResizeToContents);
    setCentralWidget(pTableView);

    connect (pHeader, &HorizontalHeader::showFilter, this, &Gui::onShowFilter);

    for (int i=0; i<LogLine::Fields::LAST; i++)
    {
        LogLine::Fields field = static_cast<LogLine::Fields>(i);
        if (LogLine::isFilterable(field))
        {
            StringFilterWidget* pFilterWidget = new StringFilterWidget(field, pModel->possibleValues[field], this);
            filterWidgets[field] = pFilterWidget;

            connect (pFilterWidget, &StringFilterWidget::filterSetAll, pProxyModel, &FilterModel::removeFilter);
            connect (pFilterWidget, &StringFilterWidget::filterSetSelection, pProxyModel, &FilterModel::addFilter);
            connect (pFilterWidget, &StringFilterWidget::filterSetRegExp, pProxyModel, &FilterModel::addFilter);
        }
    }

    connect (pModel, &Model::newFilterValue, this, &Gui::onNewFilterValue, Qt::QueuedConnection);
}

void Gui::onShowFilter(LogLine::Fields field)
{
    if (!LogLine::isFilterable(field))
        return;
    filterWidgets[field]->show();
}

void Gui::onNewFilterValue(LogLine::Fields field, const QString &value)
{
    filterWidgets[field]->addPossbleValue(value);
}

HorizontalHeader::HorizontalHeader(QWidget *parent)
    :QHeaderView(Qt::Horizontal, parent)
{

}

void HorizontalHeader::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::RightButton)
    {
        LogLine::Fields field =static_cast<LogLine::Fields>(logicalIndexAt(e->pos()));
        if (LogLine::isFilterable(field))
            emit showFilter(field);
        else
            QHeaderView::mousePressEvent(e);
    }
    else
        QHeaderView::mousePressEvent(e);
}
