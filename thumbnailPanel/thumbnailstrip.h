#ifndef THUMBNAILSTRIP_H
#define THUMBNAILSTRIP_H

#include <QLabel>
#include <QBoxLayout>
#include <QScrollArea>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QScrollBar>
#include <QMutex>
#include <QTimer>
#include <QPainter>
#include "../customWidgets/clickablelabel.h"
#include "../customWidgets/clickablewidget.h"
#include "../sourceContainers/thumbnail.h"
#include "thumbnaillabel.h"
#include "thumbnailview.h"
#include <time.h>

class ThumbnailStrip : public QWidget
{
    Q_OBJECT
public:
    explicit ThumbnailStrip();
    ~ThumbnailStrip();

private:
    QList<ThumbnailLabel*> *thumbnailLabels;
    QHash<int, long> posIdHash;
    QHash<long, int> posIdHashReverse;
    void addItem();
    QBoxLayout *layout, *buttonsLayout, *viewLayout;
    QWidget *buttonsWidget;
    ClickableLabel *openButton, *saveButton, *settingsButton, *exitButton;
    ClickableWidget *widget;
    ThumbnailFrame *thumbnailFrame;

    const qreal OPACITY_INACTIVE = 0.83;
    const qreal OPACITY_SELECTED = 1.0;
    const int ANIMATION_SPEED_INSTANT = 0;
    const int ANIMATION_SPEED_FAST = 80;
    const int ANIMATION_SPEED_NORMAL = 150;


    const uint LOAD_DELAY = 20;
    const int OFFSCREEN_PRELOAD_AREA = 1700;

    int panelSize;
    int itemCount, current, thumbnailSize, margin;
    ThumbnailStrip *strip;
    QRectF preloadArea, visibleRegion;
    QTimer loadTimer;
    QScrollBar *scrollBar;
    PanelHPosition position;
    bool parentFullscreen;
    QMutex mutex;

    void requestThumbnail(int pos);
    QGraphicsScene *scene;
    bool checkRange(int pos);
    long idCounter;
    void lock();
    void unlock();
    void updateThumbnailPositions(int start, int end);
    void shrinkScene();
signals:
    void thumbnailRequested(int pos, long thumbnailId);
    void openImage(int pos);
    void openClicked();
    void saveClicked();
    void settingsClicked();
    void exitClicked();

public slots:
    void thumbnailClicked(int pos);
    void populate(int count);
    void loadVisibleThumbnails();
    void loadVisibleThumbnailsDelayed();
    void setThumbnail(long, Thumbnail*);
    void fillPanel(int);
    void selectThumbnail(int pos);
    void setWindowControlsEnabled(bool);
    void removeItemAt(int pos);
    void addItemAt(int pos);
    void parentResized(QSize parentSz);

protected:
    virtual void paintEvent(QPaintEvent* event);
    virtual void resizeEvent(QResizeEvent *event);


private slots:
    void readSettings();
};

#endif // THUMBNAILSTRIP_H
