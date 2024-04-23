#ifndef TIMERPLAYBACK_H
#define TIMERPLAYBACK_H

#include "../models/timestep.h"
#include "qmlwidget.h"

#include <QObject>

class TimerPlayback : public QMLWidget
{
    Q_OBJECT
    Q_PROPERTY(double ruleWidth READ ruleWidth WRITE setRuleWidth NOTIFY ruleWidthChanged FINAL)
    Q_PROPERTY(double viewWidth READ viewWidth WRITE setViewWidth NOTIFY viewWidthChanged FINAL)
    Q_PROPERTY(double viewX READ viewX WRITE setViewX NOTIFY viewXChanged FINAL)
    // Q_PROPERTY(qint64 curPos READ curPos WRITE setCurPos NOTIFY curPosChanged FINAL)
    Q_PROPERTY(bool isMaximumScale READ isMaximumScale NOTIFY isMaximumScaleChanged FINAL)
    Q_PROPERTY(double mouseX READ mouseX WRITE setMouseX NOTIFY mouseXChanged FINAL)
    Q_PROPERTY(QQmlListProperty<TimeStep> timeSteps READ timeSteps NOTIFY timeStepsChanged FINAL)

    using base_type = QMLWidget;
public:
    explicit TimerPlayback(QObject *parent = nullptr,bool isInit = true);
    ~TimerPlayback();
    void setDuration(std::chrono::milliseconds duration);

    QQmlListProperty<TimeStep> timeSteps();

    double mouseX() const;
    void setMouseX(double value);

    double ruleWidth() const;
    void setRuleWidth(double width);

    double viewWidth() const;
    void setViewWidth(double value);

    double viewX() const;
    void setViewX(double value);

    // qint64 curPos() const;
    // void setCurPos(qint64 value);


    QQuickWidget* widget() const override;
    bool isMaximumScale() const;


signals:
    void ruleWidthChanged();
    void viewWidthChanged();
    void viewXChanged();
    // void curPosChanged();
    void mouseXChanged();
    void timeStepsChanged();
    void isMaximumScaleChanged();

public slots:
    // double typeDistance(RuleLine::RuleLineType type);

    // QmlWidget interface
protected:
    void registerQmlType() override;
    void setUpQmlUrl(const QUrl &componentUrl) override;
    QObject *rootObject() const override;

private:
    class Private;
    QSharedPointer<Private> d;
};

#endif // TIMERPLAYBACK_H
