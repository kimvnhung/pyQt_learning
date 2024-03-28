#include "glcontainerwidget.h"
#include "ui_glcontainerwidget.h"

GLContainerWidget::GLContainerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GLContainerWidget)
{
    ui->setupUi(this);
}

GLContainerWidget::~GLContainerWidget()
{
    delete ui;
}
