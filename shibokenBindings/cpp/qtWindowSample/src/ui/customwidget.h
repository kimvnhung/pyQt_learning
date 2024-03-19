#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QObject>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QWidget>
#include <QtAVPlayer/qavvideoframe.h>
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
public Q_SLOTS:
    void videoFrame(int index, const QAVVideoFrame &frame);
private:
    QList<QVideoWidget*> m_listWidget;
    QList<QMediaPlayer*> m_listPlayer;

};

#endif // CUSTOMWIDGET_H
