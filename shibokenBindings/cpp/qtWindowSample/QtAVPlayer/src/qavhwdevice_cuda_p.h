#ifndef QAVHWDEVICE_CUDA_P_H
#define QAVHWDEVICE_CUDA_P_H

#include "qavhwdevice_p.h"

QT_BEGIN_NAMESPACE

struct AVCodecContext;
class QAVHWDevice_CUDA : public QAVHWDevice
{
public:
    QAVHWDevice_CUDA() = default;
    ~QAVHWDevice_CUDA() = default;

    void init(AVCodecContext *avctx) override;
    AVPixelFormat format() const override;
    AVHWDeviceType type() const override;
    QAVVideoBuffer *videoBuffer(const QAVVideoFrame &frame) const override;

private:
    Q_DISABLE_COPY(QAVHWDevice_CUDA)
};

QT_END_NAMESPACE

#endif // QAVHWDEVICE_CUDA_P_H
