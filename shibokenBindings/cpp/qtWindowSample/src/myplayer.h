#ifndef MYPLAYER_H
#define MYPLAYER_H

#include "ui/customwidget.h"
#include "macros.h"

#include <QtAVPlayer/qavplayer.h>

class BINDINGS_API MyPlayer : public QObject
{
    Q_OBJECT
public:
    MyPlayer();
    MyPlayer(const MyPlayer &other);
    MyPlayer& operator=(const MyPlayer &other);
    MyPlayer(MyPlayer &&other);
    MyPlayer& operator=(MyPlayer &&other);

    ~MyPlayer();

    void play(const char* url, int configs = 0);
    void stop();
    CustomWidget* widget() const;
private:
    QList<QAVPlayer*> m_players;
    CustomWidget *m_widget;
};

#endif // MYPLAYER_H
