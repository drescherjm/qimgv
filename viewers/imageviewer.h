#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>
#include <QDesktopWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QImageReader>
#include <QMovie>
#include <QColor>
#include <QPalette>
#include <QTimer>
#include <QThread>
#include <QtConcurrent>
#include <QFuture>
#include <QDebug>
#include <QErrorMessage>
#include <vector>
#include "../sourceContainers/imagestatic.h"
#include "../overlays/mapoverlay.h"
#include "../lib/settings.h"
#include "../overlays/cropoverlay.h"
#include <time.h>
#include "../lib/imagelib.h"

#define FLT_EPSILON 1.19209290E-07F

enum ImageFitMode
{
    NORMAL,
    WIDTH,
    ALL,
    FREE
};

class ImageViewer : public QWidget
{
    Q_OBJECT

public:
    ImageViewer(QWidget* parent);
    ~ImageViewer();
    bool isDisplaying() const;

signals:
    void imageChanged();
    void cropSelected(QRect);
    void wallpaperSelected(QRect);
    void resized(QSize);
    void scalingRequested(QSize);

public slots:
    void displayImage(QPixmap* _image);
    void slotFitNormal();
    void slotFitWidth();
    void slotFitAll();
    void slotZoomIn();
    void slotZoomOut();
    void resizeImage();
    void crop();
    void readSettings();
    void hideCursor();
    void updateImage(QPixmap *scaled);
    void scrollUp();
    void scrollDown();
    void selectWallpaper();
    void showLoadingMessage();
    void startAnimation();
    void stopAnimation();
    void displayAnimation(QMovie *_animation);

protected:
    virtual void paintEvent(QPaintEvent* event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void resizeEvent(QResizeEvent* event);

private slots:
    void nextFrame();

private:
    QPixmap *image;
    QMovie *animation;
    QTransform transform;
    QTimer *resizeTimer, *cursorTimer, *animationTimer;
    QRect drawingRect;
    QPoint mouseMoveStartPos;
    QSize sourceSize;
    QColor bgColor;
    MapOverlay *mapOverlay;
    CropOverlay *cropOverlay;

    bool isDisplayingFlag, errorFlag, mouseWrapping, transparencyGridEnabled;
    const int transparencyGridSize = 10;

    float currentScale;
    float maxScale; // zoom OUT
    float minScale; // zoom IN
    float scaleStep;
    QPointF fixedZoomPoint;
    QSize desktopSize;

    ImageFitMode imageFitMode;
    void initOverlays();
    void setScale(float scale);
    void updateMaxScale();
    void scaleAround(QPointF p, float oldScale);
    void fitDefault();
    void fitNormal();
    void fitWidth();
    void fitAll();
    void centerImage();
    void updateMap();
    float scale() const;
    bool imageIsScaled() const;
    void updateMinScale();
    void alignImage();
    void fixAlignHorizontal();
    void fixAlignVertical();
    void scroll(int dx, int dy);
    void scrollX(int dx);
    void scrollY(int dy);

    void mouseDragWrapping(QMouseEvent *event);
    void mouseDrag(QMouseEvent *event);
    void mouseZoom(QMouseEvent *event);
    void drawTransparencyGrid();
    void startAnimationTimer();
    void closeImage();
    void adjustOverlays();
    void readjust(QSize _sourceSize, QRect _drawingRect);
};

#endif // IMAGEVIEWER_H
