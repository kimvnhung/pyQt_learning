#include "src/widgets/glwidget.h"
#include <QApplication>

    int main(int argc, char *argv[]){
    QApplication a(argc, argv);
        GLWidget *glWidget = new GLWidget();
    glWidget->show();
    return a.exec();
}
