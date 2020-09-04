#ifndef STRINGFILTERWIDGET_H
#define STRINGFILTERWIDGET_H

#include "logline.h"

#include <QWidget>
#include <QList>
class QCheckBox;
class QBoxLayout;
class QRegExp;

class StringFilterWidget : public QWidget
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
    explicit StringFilterWidget(LogLine::Fields field, QSet<QString> values, QWidget *parent = nullptr);

public slots:
    void addPossbleValue(const QString& value);

signals:
    void filterSetAll(LogLine::Fields field);
    void filterSetSelection(LogLine::Fields field, const QStringList& values);
    void filterSetRegExp(LogLine::Fields field, const QRegExp& regExpStr);
};

#endif // STRINGFILTERWIDGET_H
