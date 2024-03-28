#ifndef GLCONTAINERWIDGET_H
#define GLCONTAINERWIDGET_H

#include <QWidget>

namespace Ui {
class GLContainerWidget;
}

class GLContainerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GLContainerWidget(QWidget *parent = nullptr);
    ~GLContainerWidget();

private:
    Ui::GLContainerWidget *ui;
};

#endif // GLCONTAINERWIDGET_H
