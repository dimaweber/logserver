#ifndef SETFILTERWIDGET_H
#define SETFILTERWIDGET_H

#include "logline.h"

#include <QWidget>
#include <QList>
class QCheckBox;
class QBoxLayout;
class QRegExp;

class SetFilterWidget : public QWidget
{
    Q_OBJECT
    LogLine::Fields field;
    QBoxLayout* pLayout;
    QLayout* pValuesLayout;
    QWidget* pValuesWidget;
    QLayout* pRegExpLayout;
    QWidget* pRegexpWidget;
    QList<QCheckBox*> checkBoxes;
public:
    explicit SetFilterWidget(LogLine::Fields field, QSet<QString> values, QWidget *parent = nullptr);

public slots:
    void addPossbleValue(const QString& value);

signals:
    void filterSetAll(LogLine::Fields field);
    void filterSetSelection(LogLine::Fields field, const QStringList& values);
    void filterSetRegExp(LogLine::Fields field, const QRegExp& regExpStr);
};

#endif // SETFILTERWIDGET_H
