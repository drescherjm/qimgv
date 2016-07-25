#include "videoplayer.h"

using namespace QtAV;

VideoPlayer::VideoPlayer(QWidget *parent) : QWidget(parent) {
    QtAV::Widgets::registerRenderers();
    clip = new Clip();

    m_player = new AVPlayer(this);
    QVBoxLayout *vl = new QVBoxLayout();
    vl->setContentsMargins(0,0,0,0);
    setLayout(vl);
    m_vo = new VideoOutput(this);
    if (!m_vo->widget()) {
        QMessageBox::warning(0, QString::fromLatin1("QtAV error"), tr("Can not create video renderer"));
        return;
    }

    this->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_player->setRenderer(m_vo);
    m_player->setBufferValue(20000);
    m_player->setRepeat(-1);
    vl->addWidget(m_vo->widget());
    //this->setMouseTracking(true);
    this->setFocusPolicy(Qt::NoFocus);
    this->setAcceptDrops(false);

    readSettings();
    connect(settings, SIGNAL(settingsChanged()), this, SLOT(readSettings()));
}

VideoPlayer::~VideoPlayer() {
    delete clip;
}

// display & initialize
void VideoPlayer::displayVideo(Clip *_clip) {
    delete clip;
    clip = new Clip(*_clip);
    start();
}

void VideoPlayer::start() {
    stop();
    QString path = clip->getPath();
    if(!path.isEmpty()) {
        m_player->play(path);
    } else {
        qDebug() << "VideoPlayer: empty path.";
    }
}

void VideoPlayer::restart() {
    start();
}

void VideoPlayer::stop() {
    m_player->stop();
}

void VideoPlayer::readSettings() {
    //mediaPlayer.setMuted(!settings->playVideoSounds());
    //QBrush brush(settings->backgroundColor());
    //this->setBackgroundBrush(brush);
}

void VideoPlayer::mouseMoveEvent(QMouseEvent *event) {
    //QWidget::mouseMoveEvent(event);
    event->ignore();
}

void VideoPlayer::mousePressEvent(QMouseEvent *event) {
    //QWidget::mousePressEvent(event);
    event->ignore();
}

void VideoPlayer::mouseReleaseEvent(QMouseEvent *event) {
    event->ignore();
}

void VideoPlayer::wheelEvent(QWheelEvent *event) {
    event->ignore();
}
