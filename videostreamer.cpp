#include "videostreamer.h"

VideoStreamer::VideoStreamer()
{
    connect(&tUpdate,&QTimer::timeout,this,&VideoStreamer::streamVideo);

}

VideoStreamer::~VideoStreamer()
{
    cap.release();
    tUpdate.stop();
    threadStreamer->requestInterruption();
}

void VideoStreamer::streamVideo()
{

    if(frame.data)
    {
        QImage img = QImage(frame.data,frame.cols,frame.rows,QImage::Format_RGB888).rgbSwapped();
        emit newImage(img);
    }
}

void VideoStreamer::catchFrame(cv::Mat emittedFrame)
{
    frame = emittedFrame;
}

void VideoStreamer::openVideoCamera(QString path)
{
    if(path.length() == 1)
    cap.open(path.toInt());
    else
    cap.open(path.toStdString());

    VideoStreamer* worker = new VideoStreamer();
    worker->moveToThread(threadStreamer);
    QObject::connect(threadStreamer,SIGNAL(started()),worker,SLOT(streamerThreadSlot()));
    QObject::connect(worker,&VideoStreamer::emitThreadImage,this,&VideoStreamer::catchFrame);
    threadStreamer->start();

    double fps = cap.get(cv::CAP_PROP_FPS);
    tUpdate.start(1000/fps);
}

void VideoStreamer::streamerThreadSlot()
{
    cv::Mat tempFrame;

    while (1) {
        cap>>tempFrame;















        if(tempFrame.data)
            emit emitThreadImage(tempFrame);

        if(QThread::currentThread()->isInterruptionRequested())
        {
            cap.release();
            return;
        }

    }
}
