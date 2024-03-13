#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QMediaPlayer>
#include <QVideoWidget>
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
    void print(const char* msg);
    void play(const char* url);
private:
    QList<QVideoWidget*> m_listWidget;
    QList<QMediaPlayer*> m_listPlayer;

};

#endif // CUSTOMWIDGET_H
