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

    pProxyModel->addFilter(LogLine::ProcessName, "oamd");
    pProxyModel->addFilter(LogLine::Priority, 15);

    connect (pHeader, &HorizontalHeader::showFilter, this, &Gui::onShowFilter);
}

void Gui::onShowFilter(LogLine::Fields field)
{
    if (!LogLine::isFilterable(field))
        return;
    StringFilterWidget* widget = new StringFilterWidget(field, pModel->possibleValues[field], this);
    widget->show();
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
