#ifndef IMAGEFACTORY_H
#define IMAGEFACTORY_H

#include "../../lib/imagelib.h"
#include "../../lib/fileinfo.h"
#include "../../sourceContainers/image.h"
#include "../../sourceContainers/imageanimated.h"
#include "../../sourceContainers/imagestatic.h"
#include "../../sourceContainers/video.h"

class ImageFactory {
public:
    ImageFactory();

    Image* createImage(QString);
};

#endif // IMAGEFACTORY_H
