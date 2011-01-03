#ifndef ARTHANDLER_HPP
#define ARTHANDLER_HPP

#include "ImageIOHandler.hpp"

class ArtIOHandler: public ImageIOHandler {
public:
	ArtIOHandler(QIODevice* decive, const QByteArray& format = "artwork");
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
