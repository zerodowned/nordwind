/*
 * OSIImageIOHandlerPrivate.cpp
 *
 *  Created on: Sep 7, 2010
 *      Author: idstein
 */
#include "ImageIOHandler.hpp"
#include "MulFileEngine.hpp"
#include <qvariant.h>
#include <qrect.h>
#include <qimage.h>

ImageIOHandler::ImageIOHandler(QIODevice* device, const QByteArray& format)
: QImageIOHandler(), mIndex() {
	setDevice(device);
	setFormat(format);
	QFile* file = reinterpret_cast<QFile*>(device);
	if(!file)
		return;
	MulFileEngine* fileEngine = reinterpret_cast<MulFileEngine*>(file->fileEngine());
        if(fileEngine)
            fileEngine->extension((QAbstractFileEngine::Extension)Index::IndexExtension, NULL,
                                static_cast<QAbstractFileEngine::ExtensionReturn*>(&mIndex));
}

ImageIOHandler::~ImageIOHandler() {
}

bool ImageIOHandler::canRead() const {
	return mIndex.isValid();
}

bool ImageIOHandler::read(QImage* image) {
	Q_UNUSED(image);
	return false;
}

int ImageIOHandler::currentImageNumber() const {
	return 0;
}

QRect ImageIOHandler::currentImageRect() const {
	return QRect();
}

int ImageIOHandler::imageCount() const {
	return 0;
}

bool ImageIOHandler::jumpToImage(int imageNumber) {
	Q_UNUSED(imageNumber);
	return false;
}

bool ImageIOHandler::jumpToNextImage() {
	return false;
}

int ImageIOHandler::loopCount() const {
	return 0;
}

int ImageIOHandler::nextImageDelay() const {
	return 0;
}

QVariant ImageIOHandler::option(QImageIOHandler::ImageOption option) const {
	switch (option) {
	case QImageIOHandler::ImageFormat:
		return QImage::Format_ARGB32_Premultiplied;
	default:
		return QVariant();
	}
}

void ImageIOHandler::setOption(QImageIOHandler::ImageOption option,
		const QVariant& value) {
	Q_UNUSED(option);
	Q_UNUSED(value);
}

bool ImageIOHandler::supportsOption(
		QImageIOHandler::ImageOption option) const {
	Q_UNUSED(option);
	return option == QImageIOHandler::ImageFormat;
}

bool ImageIOHandler::write(const QImage& image) {
	Q_UNUSED(image);
	return false;
}
