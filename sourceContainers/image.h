#ifndef IMAGE_H
#define IMAGE_H

#include "../lib/imagelib.h"
#include "../lib/stuff.h"
#include "../lib/fileinfo.h"
#include "../lib/settings.h"
#include <QObject>
#include <QString>
#include <QIODevice>
#include <QDebug>
#include <QPixmap>
#include <QPixmapCache>
#include <QThread>
#include <QMutex>

class Image : public QObject
{
    Q_OBJECT
public:
    virtual QPixmap* getPixmap() = 0;
    virtual const QImage* getImage() = 0;
    fileType type();
    virtual void load() = 0;
    QString getPath();
    virtual int height() = 0;
    virtual int width() = 0;
    virtual QSize size() = 0;
    bool isLoaded();
    virtual QPixmap* generateThumbnail(bool) = 0;
    void attachInfo(FileInfo*);
    FileInfo* info();
    void safeDeleteSelf();

    virtual void crop(QRect newRect) = 0;
    virtual void rotate(int grad) = 0;

    virtual void lock();
    virtual void unlock();
    FileInfo* fileInfo;

protected:
    bool loaded;
    QString path;
    QSize resolution;
    QMutex mutex;

signals:

public slots:
    virtual void save() = 0;
    virtual void save(QString destinationPath) = 0;
};

#endif // IMAGE_H
