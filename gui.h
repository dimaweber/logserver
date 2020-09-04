#ifndef GUI_H
#define GUI_H

#include "logline.h"

#include <QMainWindow>
#include <QHeaderView>
#include <QMap>

class Model;
class QTableView;
class FilterModel;
class StringFilterWidget;

class HorizontalHeader : public QHeaderView
{
    Q_OBJECT
public:
    HorizontalHeader(QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *e) override;

signals:
    void showFilter(LogLine::Fields);
};

class Gui : public QMainWindow
{
    Q_OBJECT
    QTableView* pTableView;
    HorizontalHeader* pHeader;
    Model* pModel;
    FilterModel* pProxyModel;

    QMap<LogLine::Fields, StringFilterWidget*> filterWidgets;

public:
    explicit Gui(Model* pModel, QWidget *parent = nullptr);

public slots:
    void onShowFilter (LogLine::Fields field);
    void onNewFilterValue(LogLine::Fields field, const QString& value);

signals:

};

#endif // GUI_H
