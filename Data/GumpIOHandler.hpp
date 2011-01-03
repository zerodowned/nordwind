#ifndef GUMPHANDLER_HPP
#define GUMPHANDLER_HPP

#include "ImageIOHandler.hpp"

class GumpIOHandler: public ImageIOHandler {
public:
	GumpIOHandler(QIODevice* decive);
	virtual ~GumpIOHandler();
	virtual bool read(QImage *image);

	virtual QVariant option(QImageIOHandler::ImageOption option) const;
	virtual bool supportsOption(QImageIOHandler::ImageOption option) const;
};

#endif // GUMPHANDLER_HPP
