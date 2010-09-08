#include "GumpIOHandler.hpp"
#include <qdatastream.h>
#include <qimage.h>
#include <qvariant.h>

bool GumpIOHandler::read(QImage *image) {
	QDataStream stream(q_ptr->device());
	stream.setByteOrder(QDataStream::LittleEndian);
	*image = QImage(mIndex.getWidth(), mIndex.getHeight(), QImage::Format_RGB555);
	if (image->isNull())
		return false;
	QScopedArrayPointer<quint32> lookup(new quint32[mIndex.getHeight()]);
	if (stream.readRawData((char*) lookup.data(), mIndex.getHeight() << 2) != mIndex.getHeight() << 2)
		return false;
	for (quint16 y = 0; y < mIndex.getHeight(); y++) {
		// seek in stream to start position in line y
		stream.device()->seek(lookup[y] << 2);
		for( quint16 x = 0; x < mIndex.getWidth();) {
			quint16 colour, length;
			stream >> colour >> length;
			if (!colour) {
				x += length;
				continue;
			}
			for (; length > 0 && x < mIndex.getWidth(); x++, length--)
				image->setPixel(x, y, colour);
		}
	}
	return true;
}

bool GumpIOHandler::supportsOption(QImageIOHandler::ImageOption option) const {
	return option == QImageIOHandler::Size || OSIImageIOHandlerPrivate::supportsOption(option);
}

GumpIOHandler::GumpIOHandler(OSIImageIOHandler *q) :
	OSIImageIOHandlerPrivate(q) {
}

GumpIOHandler::~GumpIOHandler() {
}

QVariant GumpIOHandler::option(QImageIOHandler::ImageOption option) const {
	switch (option) {
	case QImageIOHandler::Size:
		return mIndex.getWidth() == 0 && mIndex.getHeight() == 0 ? QVariant()
				: QSize(mIndex.getWidth(), mIndex.getHeight());
	default:
		return OSIImageIOHandlerPrivate::option(option);
	}
}

