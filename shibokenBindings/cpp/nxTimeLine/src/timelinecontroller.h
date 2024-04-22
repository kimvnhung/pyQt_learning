#ifndef TIMELINECONTROLLER_H
#define TIMELINECONTROLLER_H

#include <QObject>
#include <QSharedPointer>
#include "macros.h"

class BINDINGS_API TimeLineController : public QObject
{
    Q_OBJECT
public:
    TimeLineController();
    TimeLineController(const TimeLineController &other);
    TimeLineController& operator=(const TimeLineController &other);
    TimeLineController(TimeLineController &&other);
    TimeLineController& operator=(TimeLineController &&other);

    ~TimeLineController();

private:
    class Private;
    QSharedPointer<Private> d;
};

#endif // TIMELINECONTROLLER_H
