#include "avplayermanager.h"

AVPlayerManager::AVPlayerManager(QStringList urls)
{
    qDebug()<<urls;
    for (int i=0;i<urls.size();i++) {
        QAVPlayer *player = new QAVPlayer(this);
        // player->setFilter("scale='min(320,iw)':'min(240,ih)'");
        player->setSource(urls.at(i));
        connect(player, &QAVPlayer::videoFrame,[this,i](const QAVVideoFrame &frame){
            emit videoFrame(i,(void*)&frame);
        });
        m_players.append(player);
    }
}

AVPlayerManager::AVPlayerManager(const AVPlayerManager &other){
    // assign(other);
}

AVPlayerManager& AVPlayerManager::operator=(const AVPlayerManager &other){
    if (this != &other) {

    }
    return *this;
}

AVPlayerManager::AVPlayerManager(AVPlayerManager &&other){

}

AVPlayerManager& AVPlayerManager::operator=(AVPlayerManager &&other){
    return *this;
}

AVPlayerManager::~AVPlayerManager()
{

}

void AVPlayerManager::play()
{
    qDebug()<<__FUNCTION__;
    foreach (QAVPlayer* p, m_players) {
        p->play();
    }
}
