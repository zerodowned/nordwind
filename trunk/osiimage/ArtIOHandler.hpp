#ifndef ARTHANDLER_HPP
#define ARTHANDLER_HPP

#include "OSIImageIOHandlerPrivate.hpp"

class ArtIOHandler: public OSIImageIOHandlerPrivate {
public:
	ArtIOHandler(OSIImageIOHandler* q);
	virtual ~ArtIOHandler();
	virtual bool read(QImage *image);

	virtual QVariant option(QImageIOHandler::ImageOption option) const;
	virtual bool supportsOption(QImageIOHandler::ImageOption option) const;
private:
	quint32 mFlags;
	quint16 mWidth;
	quint16 mHeight;
};

#endif // ARTHANDLER_HPP
