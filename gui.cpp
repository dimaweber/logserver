#include "gui.h"
#include "filtermodel.h"
#include "setfilterwidget.h"
#include "model.h"

#include <QTableView>
#include <QHeaderView>
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QMenu>

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
    pTableView->setContextMenuPolicy(Qt::CustomContextMenu);
    pHeader->setStretchLastSection(true);
    pHeader->setSectionResizeMode(QHeaderView::ResizeToContents);
    pHeader->setContextMenuPolicy(Qt::NoContextMenu);
    setCentralWidget(pTableView);

    for (int i=0; i<LogLine::Fields::LAST; i++)
    {
        LogLine::Fields field = static_cast<LogLine::Fields>(i);
        QMenu *menu=new QMenu(this);
        menu->addAction("Hide this line", this, &Gui::hideLine);

        if (field == LogLine::Fields::Message)
        {
            menu->addAction("Hide all lines with same message", this, &Gui::hideSameLines);
            menu->addAction(new QAction("Copy message to clipboard", this));
        }
        contextMenuForFields[field] = menu;
    }

    connect (pHeader, &HorizontalHeader::showFilter, this, &Gui::onShowFilter);
    connect (pTableView, &QTableView::customContextMenuRequested, this, &Gui::contextMenuRequested);

    for (int i=0; i<LogLine::Fields::LAST; i++)
    {
        LogLine::Fields field = static_cast<LogLine::Fields>(i);
        if (LogLine::isSetFilter(field))
        {
            SetFilterWidget* pFilterWidget = new SetFilterWidget(field, pModel->possibleValues[field], this);
            setFilterWidgets[field] = pFilterWidget;

            connect (pFilterWidget, &SetFilterWidget::filterSetAll, pProxyModel, &FilterModel::removeFilter);
            connect (pFilterWidget, &SetFilterWidget::filterSetSelection, pProxyModel, &FilterModel::addFilter);
            connect (pFilterWidget, &SetFilterWidget::filterSetRegExp, pProxyModel, &FilterModel::addFilter);
        }
        if (LogLine::isMinMaxFilter(field))
        {
            QWidget* pFilterWidget = new QWidget(this);
            minmaxFilterWidgets[field] = pFilterWidget;
        }
    }

    connect (pModel, &Model::newFilterValue, this, &Gui::onNewFilterValue, Qt::QueuedConnection);
}

void Gui::onShowFilter(LogLine::Fields field)
{
    if (LogLine::isSetFilter(field))
        setFilterWidgets[field]->show();
    if (LogLine::isMinMaxFilter(field))
        minmaxFilterWidgets[field]->show();

}

void Gui::onNewFilterValue(LogLine::Fields field, const QString &value)
{
    setFilterWidgets[field]->addPossbleValue(value);
}

void Gui::contextMenuRequested(QPoint pos)
{
    QModelIndex index=pTableView->indexAt(pos);

    QMenu *menu=contextMenuForFields[static_cast<LogLine::Fields>(index.column())];
    if (menu)
        menu->popup(pTableView->viewport()->mapToGlobal(pos));
}

void Gui::hideLine()
{
    QModelIndex idx = pTableView->currentIndex();
    QModelIndex modelIdx = pProxyModel->mapToSource(idx);
    qDebug() << "hide row " << modelIdx.row();
    pProxyModel->hideRow(modelIdx);
}

void Gui::hideSameLines()
{
    QString text = pProxyModel->data(pTableView->currentIndex()).toString();
    qDebug() << "hide rows " << text;
    pProxyModel->hideRowsWithText(text);
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
