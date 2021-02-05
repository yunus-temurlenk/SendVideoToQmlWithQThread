#ifndef VIDEOSTREAMER_H
#define VIDEOSTREAMER_H

#include <QObject>
#include <QTimer>
#include <opencv2/highgui.hpp>
#include <QImage>
#include <iostream>
#include <QThread>


static cv::VideoCapture cap;


class VideoStreamer: public QObject
{
    Q_OBJECT
public:
    VideoStreamer();
    ~VideoStreamer();

public:
    void streamVideo();
    QThread* threadStreamer = new QThread();
    void catchFrame(cv::Mat emittedFrame);

public slots:
    void openVideoCamera(QString path);

    void streamerThreadSlot();

private:
    cv::Mat frame;
    QTimer tUpdate;

signals:
    void newImage(QImage &);

    void emitThreadImage(cv::Mat frameThread);
};

#endif // VIDEOSTREAMER_H
