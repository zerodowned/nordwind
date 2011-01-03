/*
 * OSIImageIOHandlerPrivate.hpp
 *
 *  Created on: Sep 7, 2010
 *      Author: idstein
 */

#ifndef OSIIMAGEIOHANDLERPRIVATE_HPP_
#define OSIIMAGEIOHANDLERPRIVATE_HPP_

#include <QImageIOHandler>
#include "Index.hpp"
#include <qrgb.h>

/*
 *
 */
class ImageIOHandler : public QImageIOHandler {
public:
	ImageIOHandler(QIODevice* device, const QByteArray& format);
	virtual ~ImageIOHandler();
	virtual bool	canRead () const;
	virtual bool	read ( QImage * image );
	virtual int	currentImageNumber () const;
	virtual QRect	currentImageRect () const;
	virtual int	imageCount () const;
	virtual bool	jumpToImage ( int imageNumber );
	virtual bool	jumpToNextImage ();
	virtual int	loopCount () const;
	virtual int	nextImageDelay () const;
	virtual QVariant	option ( QImageIOHandler::ImageOption option ) const;
	virtual void	setOption ( QImageIOHandler::ImageOption option, const QVariant & value );
	virtual bool	supportsOption ( QImageIOHandler::ImageOption option ) const;
	virtual bool	write ( const QImage & image );
protected:
	Index mIndex;
};

inline QRgb qrgb555(quint16 color16) {
    return qRgb((color16>>7)&0xF8,(color16>>2)&0xF8,(color16<<3)&0xF8);
}

#endif /* OSIIMAGEIOHANDLERPRIVATE_HPP_ */
