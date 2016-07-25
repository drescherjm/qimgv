#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QWidget>
#include <QMouseEvent>
#include <QDebug>
#include "../sourceContainers/clip.h"
#include "../settings.h"
#include <QtAV/QtAV>
#include <QtAVWidgets/QtAVWidgets>
#include <QVBoxLayout>
#include <QMessageBox>

class VideoPlayer : public QWidget
{
    Q_OBJECT
public:
    VideoPlayer(QWidget *parent = 0);
    ~VideoPlayer();

signals:
    void parentResized(QSize);

public slots:
    void displayVideo(Clip *clip);
    void start();
    void restart();
    void stop();
    void readSettings();

private:
    Clip *clip;

    // QtAV
    QtAV::VideoOutput *m_vo;
    QtAV::AVPlayer *m_player;

protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
};

#endif // VIDEOPLAYER_H
