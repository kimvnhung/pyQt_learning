#include "customwidget.h"

#include <QGridLayout>
#include <QLabel>

CustomWidget::CustomWidget(QWidget *parent)
    : QWidget{parent}
{
    QGridLayout *layout = new QGridLayout(this);
    QLabel *lable = new QLabel("Hello From Custom Widget",this)

}
