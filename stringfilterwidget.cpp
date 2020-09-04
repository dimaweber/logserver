#include "stringfilterwidget.h"
#include <QBoxLayout>
#include <QRadioButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QSet>


StringFilterWidget::StringFilterWidget(LogLine::Fields field, QSet<QString> values, QWidget *parent)
    : QWidget(parent, Qt::Window)
{
    QBoxLayout* l = new QVBoxLayout(this);
    setLayout(l);

    QRadioButton* pAllRadio = new QRadioButton("All", this);
    QRadioButton* pSelectRadio = new QRadioButton("Selected", this);
    QRadioButton* pRegExpRadio = new QRadioButton("RegExp", this);
    QLineEdit*    pRegExpEdit = new QLineEdit(this);

    l->addWidget(pAllRadio);
    l->addWidget(pSelectRadio);
    for (QString v: values)
        l->addWidget( new QCheckBox(v, this));
    l->addWidget(pRegExpRadio);
    l->addWidget(pRegExpEdit);
}
