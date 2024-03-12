#include "customwidget.h"

#include <QGridLayout>
#include <QLabel>

CustomWidget::CustomWidget()
{
    QGridLayout *layout = new QGridLayout(this);
    QLabel *lable = new QLabel("Hello Fromffsadasdast",this);
    QLabel *lable2 = new QLabel("Lable2",this);
    layout->addWidget(lable);
    layout->addWidget(lable2);
    setLayout(layout);
}

CustomWidget::CustomWidget(const CustomWidget &other){
    // assign(other);
}

CustomWidget& CustomWidget::operator=(const CustomWidget &other){
    if (this != &other) {

    }
    return *this;
}

CustomWidget::CustomWidget(CustomWidget &&other){

}

CustomWidget& CustomWidget::operator=(CustomWidget &&other){
    return *this;
}

CustomWidget::~CustomWidget(){

}
