#include "setfilterwidget.h"
#include <QBoxLayout>
#include <QRadioButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QSet>
#include <QRegExp>

SetFilterWidget::SetFilterWidget(LogLine::Fields f, QSet<QString> values, QWidget *parent)
    : QWidget(parent, Qt::Window), field(f)
{
    setWindowTitle(LogLine::fieldName(field));

    pLayout = new QVBoxLayout(this);
    setLayout(pLayout);

    QRadioButton* pAllRadio = new QRadioButton("All", this);
    pLayout->addWidget(pAllRadio);
    connect (pAllRadio, &QRadioButton::toggled, [this](bool checked)
    {
        if (checked)
        {
            pValuesWidget->setEnabled(false);
            pRegexpWidget->setEnabled(false);

            emit filterSetAll(field);
        }
    });

    QRadioButton* pSelectRadio = new QRadioButton("Selected", this);
    pLayout->addWidget(pSelectRadio);
    pValuesWidget = new QWidget(this);
    pValuesLayout = new QVBoxLayout(pValuesWidget);
    pValuesWidget->setLayout(pValuesLayout);
    for (const QString& v: values)
        addPossbleValue(v);
    pLayout->addWidget(pValuesWidget);
    connect (pSelectRadio, &QRadioButton::toggled, [this](bool checked)
    {
       if (checked)
       {
           pRegexpWidget->setEnabled(false);
           pValuesWidget->setEnabled(true);

           QStringList values;
           for (QCheckBox* pCheck: checkBoxes)
           {
               if (pCheck->isChecked())
                    values.append(pCheck->text());
           }

           emit filterSetSelection(field, values);
       }
    });

    QRadioButton* pRegExpRadio = new QRadioButton("RegExp", this);
    pRegexpWidget = new QWidget(this);
    pRegExpLayout = new QHBoxLayout(pRegexpWidget);
    pRegexpWidget->setLayout(pRegExpLayout);
    QLineEdit*    pRegExpEdit = new QLineEdit(pRegexpWidget);
    pRegExpLayout->addWidget(pRegExpEdit);
    pLayout->addWidget(pRegExpRadio);
    pLayout->addWidget(pRegexpWidget);
    connect (pRegExpRadio, &QRadioButton::toggled,  [this, pRegExpEdit](bool checked)
    {
        if (checked)
        {
            pRegexpWidget->setEnabled(true);
            pValuesWidget->setEnabled(false);

            emit filterSetRegExp(field, QRegExp(pRegExpEdit->text()));
        }
    });
    connect (pRegExpEdit, &QLineEdit::textChanged, [this, pRegExpEdit]()
    {
        emit filterSetRegExp(field, QRegExp(pRegExpEdit->text()));
    });

    pAllRadio->setChecked(true);
}

void SetFilterWidget::addPossbleValue(const QString &value)
{
    auto pCheckBox = new QCheckBox(value, pValuesWidget);
    pValuesWidget->layout()->addWidget( pCheckBox);
    checkBoxes.append(pCheckBox);

    connect (pCheckBox, &QCheckBox::toggled, [this]()
    {
       QStringList values;
       for (QCheckBox* pCheck: checkBoxes)
       {
           if (pCheck->isChecked())
                values.append(pCheck->text());
       }

       emit filterSetSelection(field, values);
    });

    pCheckBox->setChecked(true);
}
