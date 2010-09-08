#include "AnimationIOHandler.hpp"
#include <qimage.h>
#include <qvariant.h>

int AnimationIOHandler::sNextImageDelay = 100;

inline quint8 red(quint16 _colour16) {
	return ((_colour16 >> 10) & 0x1F);
}

inline quint8 qRed(quint16 _colour16) {
	return red(_colour16) << 3;
}

inline quint8 green(quint16 _colour16) {
	return ((_colour16 >> 5) & 0x1F);
}

inline quint8 qGreen(quint16 _colour16) {
	return green(_colour16) << 3;
}

inline quint8 blue(quint16 _colour16) {
	return (_colour16 & 0x1F);
}

inline quint8 qBlue(quint16 _colour16) {
	return blue(_colour16) << 3;
}
inline QRgb qRgb(quint16 _colour16) {
	return qRgb(qRed(_colour16), qGreen(_colour16), qBlue(_colour16));
}

AnimationIOHandler::~AnimationIOHandler() {
}

bool AnimationIOHandler::read(QImage *image) {
	QDataStream stream(q_ptr->device());
	stream.setByteOrder(QDataStream::LittleEndian);
	if (mImageCount == -1) {
		QScopedArrayPointer<quint16> colorTable(new quint16[0xFF]);
		stream.readRawData((char*)colorTable.data(), 0xFF << 1);
		for (int i = 0; i < 0xFF; i++)
			mColorTable[i] = qRgb(colorTable[i]);
		stream >> mImageCount;
		mImageOffset.resize(mImageCount);
		stream.readRawData((char*) mImageOffset.data(), sizeof(quint32)
				* mImageCount);
		if(!jumpToImage(0))
			return false;
	}
	qint16 centerX, centerY;
	stream >> centerX >> centerY;
	quint16 width, height;
	stream >> width >> height;
	mCurrentImageRect.setRect(centerX,centerY,width,height);
	*image = QImage(width, height, QImage::Format_Indexed8);
	if (image->isNull())
		return false;
	image->setColorTable(mColorTable);
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
			image->setPixel(x, y, pixel);
		}
	} while (true);
	return true;
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
	return 0;
}

int AnimationIOHandler::imageCount() const {
	return mImageCount;
}

bool AnimationIOHandler::jumpToImage(int imageNumber) {
	if (q_ptr->device()->seek(mImageOffset[imageNumber])) {
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

AnimationIOHandler::AnimationIOHandler(OSIImageIOHandler *p) :
	OSIImageIOHandlerPrivate(p), mImageCount(0), mCurrentImageNumber(-1),
			mColorTable(0xFF) {
}

bool AnimationIOHandler::supportsOption(QImageIOHandler::ImageOption option) const {
	return option == QImageIOHandler::Size || option
			== QImageIOHandler::Animation
			|| OSIImageIOHandlerPrivate::supportsOption(option);
}

QVariant AnimationIOHandler::option(QImageIOHandler::ImageOption option) const {
	switch (option) {
	case QImageIOHandler::Size:
		return mCurrentImageRect.isNull() ? QVariant()
				: mCurrentImageRect.size();
	default:
		return OSIImageIOHandlerPrivate::option(option);
	}
}
