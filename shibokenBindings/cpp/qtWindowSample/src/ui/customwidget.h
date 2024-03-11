#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QWidget>
#include "../macros.h"

class BINDINGS_API CustomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CustomWidget(QWidget *parent = nullptr);
    // ~CustomWidget();

signals:
};

#endif // CUSTOMWIDGET_H
