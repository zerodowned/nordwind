#include "AnimationIOHandler.hpp"
#include <qimage.h>
#include <qvariant.h>
#include <qdebug.h>

int AnimationIOHandler::sNextImageDelay = 100;

AnimationIOHandler::~AnimationIOHandler() {
}

bool AnimationIOHandler::read(QImage *image) {
	if(!device()->seek(0x200+mImageOffset[mCurrentImageNumber]))
		return false;
	QDataStream stream(device());
	stream.setByteOrder(QDataStream::LittleEndian);
	qint16 centerX, centerY;
	stream >> centerX >> centerY;
	qint16 width, height;
	stream >> width >> height;
	mCurrentImageRect.setRect(centerX,centerY,width,height);
	*image = QImage(width, height, QImage::Format_ARGB32_Premultiplied);
	if (image->isNull())
		return false;
	image->fill(0);
	// Start reading the frame runs
	do {
		quint32 header = 0x7FFF7FFF;
		stream >> header; // Read the frame header

		// This is the end marker
		if (header == 0x7FFF7FFF)
			break;

		// Unpack the header
		qint16 xoffset, yoffset;
		quint16 length; // Chunk properties
		xoffset = (header >> 22) & 0x3FF;
		yoffset = (header >> 12) & 0x3FF;
		length = header & 0xFFF;

		if (xoffset & 0x200)
			xoffset |= 0xFC00; // This is suboptimal
		if (yoffset & 0x200)
			yoffset |= 0xFC00; // This is suboptimal

		qint32 y = centerY + yoffset + height;
		qint32 x = centerX + xoffset;
		for (quint16 i = 0; i < length; i++, x++) {
			quint8 pixel;
			stream >> pixel;
			image->setPixel(x, y, mColorTable[pixel]);
		}
	} while (true);
	mCurrentImageNumber++;
	return true;
}

bool AnimationIOHandler::canRead() const {
	return mCurrentImageNumber<mImageCount; 
}

QRect AnimationIOHandler::currentImageRect() const {
	return mCurrentImageRect;
}

int AnimationIOHandler::currentImageNumber() const {
	return mCurrentImageNumber;
}

int AnimationIOHandler::nextImageDelay() const {
	return sNextImageDelay;
}

int AnimationIOHandler::loopCount() const {
	return -1;
}

int AnimationIOHandler::imageCount() const {
	return mImageCount;
}

bool AnimationIOHandler::jumpToImage(int imageNumber) {
	if(imageNumber<mImageOffset.size()) {
		mCurrentImageNumber = imageNumber;
		return true;
	} else
		return false;
}

bool AnimationIOHandler::jumpToNextImage() {
	if (mCurrentImageNumber < mImageCount) {
		return jumpToImage(mCurrentImageNumber++);
	} else
		return false;
}

AnimationIOHandler::AnimationIOHandler(QIODevice* device) :
	ImageIOHandler(device,"animation"), mImageCount(0), mCurrentImageNumber(-1),
			mColorTable(256) {
		QDataStream stream(device);
		stream.setByteOrder(QDataStream::LittleEndian);
		quint16 colour;
		for (QVector<QRgb>::iterator iter = mColorTable.begin(); iter < mColorTable.end(); iter++) {
			stream >> colour;
			*iter = qRgb((colour>>7)&0xF8,(colour>>2)&0xF8,(colour<<3)&0xF8);
		}
		stream >> mImageCount;
		mImageOffset.resize(mImageCount);
		for (QVector<qint32>::iterator iter = mImageOffset.begin(); iter < mImageOffset.end(); iter++)
			stream >> *iter;
		mCurrentImageNumber = 0;
}

bool AnimationIOHandler::supportsOption(QImageIOHandler::ImageOption option) const {
	return option == QImageIOHandler::Size || option
			== QImageIOHandler::Animation
			|| ImageIOHandler::supportsOption(option);
}

QVariant AnimationIOHandler::option(QImageIOHandler::ImageOption option) const {
	switch (option) {
	case QImageIOHandler::Size:
		return mCurrentImageRect.isNull() ? QVariant()
				: mCurrentImageRect.size();
	default:
		return ImageIOHandler::option(option);
	}
}
