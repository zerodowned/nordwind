#ifndef GUMPHANDLER_HPP
#define GUMPHANDLER_HPP

#include "OSIImageIOHandlerPrivate.hpp"

class GumpIOHandler: public OSIImageIOHandlerPrivate {
public:
	GumpIOHandler(OSIImageIOHandler* q);
	virtual ~GumpIOHandler();
	virtual bool read(QImage *image);

	virtual QVariant option(QImageIOHandler::ImageOption option) const;
	virtual bool supportsOption(QImageIOHandler::ImageOption option) const;
};

#endif // GUMPHANDLER_HPP
