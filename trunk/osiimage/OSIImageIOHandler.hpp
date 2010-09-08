/*
 * OSIImageIOHandler.hpp
 *
 *  Created on: Sep 7, 2010
 *      Author: idstein
 */

#ifndef OSIIMAGEIOHANDLER_HPP_
#define OSIIMAGEIOHANDLER_HPP_

#include <qimageiohandler.h>

class OSIImageIOHandlerPrivate;
/*
 *
 */
class OSIImageIOHandler : public QImageIOHandler{
	Q_DECLARE_PRIVATE(OSIImageIOHandler);
public:
	static bool canRead(QIODevice* device, const QByteArray& format);
	OSIImageIOHandler(QIODevice* device, const QByteArray& format);
	virtual ~OSIImageIOHandler();
	virtual bool	canRead () const;
	virtual bool	read ( QImage * image );
	virtual int	currentImageNumber () const;
	virtual QRect	currentImageRect () const;
	virtual int	imageCount () const;
	virtual bool	jumpToImage ( int imageNumber );
	virtual bool	jumpToNextImage ();
	virtual int	loopCount () const;
	virtual int	nextImageDelay () const;
	virtual QVariant	option ( ImageOption option ) const;
	void setFormat( const QByteArray & format );
	virtual void	setOption ( ImageOption option, const QVariant & value );
	virtual bool	supportsOption ( ImageOption option ) const;
	virtual bool	write ( const QImage & image );
private:
	QScopedPointer<OSIImageIOHandlerPrivate> d_ptr;
private:
    Q_DISABLE_COPY(OSIImageIOHandler);
};

#endif /* OSIIMAGEIOHANDLER_HPP_ */
