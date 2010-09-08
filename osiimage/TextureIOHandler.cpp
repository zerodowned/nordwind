#include "TextureIOHandler.hpp"
#include <qdatastream.h>
#include <qimage.h>
#include <qvariant.h>

bool TextureIOHandler::read(QImage *image) {
	QDataStream stream(q_ptr->device());
	stream.setByteOrder(QDataStream::LittleEndian);
	if(size().isNull())
		return false;
	*image = QImage(size(), QImage::Format_RGB555);
	if (image->isNull())
		return false;
	QScopedArrayPointer<quint16> raw(new quint16[mIndex.mSize]);
	quint16 width = size().width();
	if (stream.readRawData((char*) raw.data(), mIndex.mSize<<1) != mIndex.mSize<<1)
		return false;
	for (quint16 i = 0; i < mIndex.mSize; i++)
		image->setPixel(i % width, i / width, raw[i]);
	return true;
}

bool TextureIOHandler::supportsOption(QImageIOHandler::ImageOption option) const {
	return option == QImageIOHandler::Size || OSIImageIOHandlerPrivate::supportsOption(option);
}

TextureIOHandler::TextureIOHandler(OSIImageIOHandler *q) :
	OSIImageIOHandlerPrivate(q) {
}

TextureIOHandler::~TextureIOHandler() {
}

QVariant TextureIOHandler::option(QImageIOHandler::ImageOption option) const {
	switch (option) {
	case QImageIOHandler::Size:
		return size().isNull() ? QVariant()
				: size();
	default:
		return OSIImageIOHandlerPrivate::option(option);
	}
}

QSize TextureIOHandler::size() const {
	if(mIndex.mSize==0x2000)
		return QSize(64,64);
	else if(mIndex.mSize==0x8000)
		return QSize(128,128);
	else
		return QSize();
}
