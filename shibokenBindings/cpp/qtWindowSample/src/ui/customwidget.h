#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QWidget>
#include "../macros.h"

class BINDINGS_API CustomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CustomWidget();
    CustomWidget(const CustomWidget &other);
    CustomWidget& operator=(const CustomWidget &other);
    CustomWidget(CustomWidget &&other);
    CustomWidget& operator=(CustomWidget &&other);

    ~CustomWidget();

};

#endif // CUSTOMWIDGET_H
