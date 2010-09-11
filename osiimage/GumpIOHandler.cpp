#include "GumpIOHandler.hpp"
#include <qdatastream.h>
#include <qimage.h>
#include <qvariant.h>
#include <QtAlgorithms>
#include <private/qdrawhelper_p.h>

bool GumpIOHandler::read(QImage *image) {
	QDataStream stream(q_ptr->device());
	stream.setByteOrder(QDataStream::LittleEndian);
	*image = QImage(mIndex.getWidth(), mIndex.getHeight(), QImage::Format_ARGB32_Premultiplied);
	if (image->isNull())
		return false;
	image->fill(0);
	QVector<quint32> lookup(mIndex.getHeight());
	for(QVector<quint32>::iterator iter = lookup.begin(); iter!=lookup.end();iter++)
		stream >> *iter;
	QRgb* d = (QRgb*)image->bits();
	int bytesPerLine = image->bytesPerLine();
	for (quint16 y = 0; y < lookup.size(); y++) {
		// seek in stream to start position in line y
		stream.device()->seek(lookup[y] << 2);
		for( quint16 x = 0; x < mIndex.getWidth();) {
			quint16 colour, length;
			stream >> colour >> length;
			if (!colour)
				x += length;
			else
				for(QRgb rgb = qrgb555(colour); length > 0 && x < mIndex.getWidth(); x++, length--)
					d[y*bytesPerLine/sizeof(QRgb)+x] = rgb;
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

