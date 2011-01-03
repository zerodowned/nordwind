#ifndef TEXTUREHANDLER_HPP
#define TEXTUREHANDLER_HPP

#include "ImageIOHandler.hpp"

class TextureIOHandler: public ImageIOHandler {
public:
	TextureIOHandler(QIODevice* decive);
	virtual ~TextureIOHandler();
	virtual bool read(QImage *image);
	QSize size() const;
	virtual QVariant option(QImageIOHandler::ImageOption option) const;
	virtual bool supportsOption(QImageIOHandler::ImageOption option) const;
};

#endif // TEXTUREHANDLER_HPP
