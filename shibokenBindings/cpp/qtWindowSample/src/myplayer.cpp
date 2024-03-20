#include "myplayer.h"

MyPlayer::MyPlayer() {
    if(m_widget == nullptr){
        m_widget = new CustomWidget;
    }
}

MyPlayer::MyPlayer(const MyPlayer &other){
    // assign(other);
}

MyPlayer& MyPlayer::operator=(const MyPlayer &other){
    if (this != &other) {

    }
    return *this;
}

MyPlayer::MyPlayer(MyPlayer &&other){

}

MyPlayer& MyPlayer::operator=(MyPlayer &&other){
    return *this;
}

MyPlayer::~MyPlayer(){

}

void MyPlayer::play(const char* url, int configs)
{
    qDebug()<<__FUNCTION__<<"on play "<<url;
    m_widget->addCamera();
    QAVPlayer *player = new QAVPlayer(this);
    player->setSource(url,configs);
    int index = m_players.size();
    connect(player,&QAVPlayer::videoFrame,[this,index](const QAVVideoFrame &frame){
        m_widget->videoFrame(index,frame);
    });

    m_players.append(player);
    player->play();
}

CustomWidget* MyPlayer::widget() const
{
    return m_widget;
}

void MyPlayer::stop(){
    for(int i=0;i<m_players.size();i++){
        m_players[i]->stop();
    }
}

