#include "TextureIOHandler.hpp"
#include <qdatastream.h>
#include <qimage.h>
#include <qvariant.h>
#include <private/qdrawhelper_p.h>

bool TextureIOHandler::read(QImage *image) {
	QDataStream stream(q_ptr->device());
	stream.setByteOrder(QDataStream::LittleEndian);
	if(!size().isValid())
		return false;
	*image = QImage(size(), QImage::Format_ARGB32_Premultiplied);
	if (image->isNull())
		return false;
	memset((void*)image->bits(),0,image->byteCount());
	QVector<quint16> raw(size().width()*size().height());
	QRgb* d = (QRgb*)image->bits();
	for(QVector<quint16>::iterator iter = raw.begin(); iter!=raw.end();iter++, d++) {
		stream >> *iter;
		*d = qRgb(((*iter)>>7)&0xF8,((*iter)>>2)&0xF8,((*iter)<<3)&0xF8);
		//((*iter)<<3)|(((*iter)&0x3E0)<<3)|(*iter)&0x1F;//qrgb555(*iter);
	}
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
		return !size().isValid() ? QVariant()
				: size();
	default:
		return OSIImageIOHandlerPrivate::option(option);
	}
}

QSize TextureIOHandler::size() const {
	if(mIndex.mExtra==1)
		return QSize(128,128);
	else
		return QSize(64,64);
}
