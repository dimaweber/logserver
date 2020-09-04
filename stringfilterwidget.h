#ifndef STRINGFILTERWIDGET_H
#define STRINGFILTERWIDGET_H

#include "logline.h"

#include <QWidget>

class StringFilterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StringFilterWidget(LogLine::Fields field, QSet<QString> values, QWidget *parent = nullptr);

signals:

};

#endif // STRINGFILTERWIDGET_H
