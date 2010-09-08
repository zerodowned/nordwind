#ifndef ANIMATIONIOHANDLER_HPP
#define ANIMATIONIOHANDLER_HPP

#include "OSIImageIOHandlerPrivate.hpp"
#include <qvector.h>
#include <qrect.h>
#include <qcolor.h>

class AnimationIOHandler : public OSIImageIOHandlerPrivate
{
    public:
    AnimationIOHandler(OSIImageIOHandler* p);
    ~AnimationIOHandler();
    virtual bool read(QImage *image);
    virtual int currentImageNumber() const;
    virtual QRect	currentImageRect () const;
    virtual int imageCount() const;
    virtual bool jumpToImage(int imageNumber);
    virtual bool jumpToNextImage();
    virtual int loopCount() const;
    virtual int nextImageDelay() const;
    virtual QVariant option(QImageIOHandler::ImageOption option) const;
    virtual bool supportsOption(QImageIOHandler::ImageOption option) const;

    private:
    	int mImageCount;
    	int mCurrentImageNumber;
    	QVector<QRgb> mColorTable;
    	QRect mCurrentImageRect;
    	QVector<quint32> mImageOffset;
    	static int sNextImageDelay;
};

#endif // ANIMATIONIOHANDLER_HPP
