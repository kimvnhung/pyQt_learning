#ifndef AVPLAYERMANAGER_H
#define AVPLAYERMANAGER_H

#include <QtAVPlayer/qavplayer.h>
#include <QObject>

class AVPlayerManager : public QObject
{
    Q_OBJECT
public:
    explicit AVPlayerManager(QStringList urls = QStringList());
    AVPlayerManager(const AVPlayerManager &other);
    AVPlayerManager& operator=(const AVPlayerManager &other);
    AVPlayerManager(AVPlayerManager &&other);
    AVPlayerManager& operator=(AVPlayerManager &&other);
    ~AVPlayerManager();

Q_SIGNALS:
    void videoFrame(int index, const void *frame);
public Q_SLOTS:
    void play();
private:
    QList<QAVPlayer*> m_players;
};
#endif // AVPLAYERMANAGER_H
