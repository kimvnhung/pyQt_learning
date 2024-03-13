#include "customwidget.h"

#include <QGridLayout>
#include <QLabel>

CustomWidget::CustomWidget()
{
    QGridLayout *layout = new QGridLayout(this);
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

void CustomWidget::print(const char* msg)
{
    qDebug()<<msg;
}

void CustomWidget::play(const char* url)
{
    qDebug()<<__FUNCTION__;
    QMediaPlayer *m_player = new QMediaPlayer(this);
    QVideoWidget *m_videoWidget = new QVideoWidget(this);
    m_videoWidget->resize(400,300);
    m_player->setVideoOutput(m_videoWidget);
    m_listPlayer.append(m_player);
    m_listWidget.append(m_videoWidget);
    layout()->addWidget(m_videoWidget);
    m_player->setSource(QUrl(url));
    m_player->setPlaybackRate(1);
    m_player->play();
}
