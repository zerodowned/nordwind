/*
 * OSIImageIOHandlerPrivate.hpp
 *
 *  Created on: Sep 7, 2010
 *      Author: idstein
 */

#ifndef OSIIMAGEIOHANDLERPRIVATE_HPP_
#define OSIIMAGEIOHANDLERPRIVATE_HPP_

#include "OSIImageIOHandler.hpp"
#include "../nordwind/resource/Index.hpp"
/*
 *
 */
class OSIImageIOHandlerPrivate {
	Q_DECLARE_PUBLIC(OSIImageIOHandler);
public:
	OSIImageIOHandlerPrivate(OSIImageIOHandler* q);
	virtual ~OSIImageIOHandlerPrivate();
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
	OSIImageIOHandler* q_ptr;
	resource::Index mIndex;
};

#endif /* OSIIMAGEIOHANDLERPRIVATE_HPP_ */
