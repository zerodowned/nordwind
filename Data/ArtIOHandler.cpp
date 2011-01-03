#include "ArtIOHandler.hpp"
#include <qdatastream.h>
#include <qimage.h>
#include <qvariant.h>
#include <qdebug.h>

bool ArtIOHandler::read(QImage *image) {
	QDataStream stream(device());
	stream.setByteOrder(QDataStream::LittleEndian);
	if (format() == "map")
		mWidth = mHeight = 44;
	else {
		stream >> mFlags; //flags
		if(mFlags>0xFFFF||mFlags==0) {
			mWidth = mHeight = 44;
			setFormat("map");
		} else
			stream >> mWidth >> mHeight;
	}
	*image = QImage(mWidth, mHeight, QImage::Format_ARGB32_Premultiplied);
	if (image->isNull())
		return false;
	memset((void*)image->bits(),0,image->byteCount());
	QRgb* d = (QRgb*)(image->bits());
	int bytesPerLine = image->bytesPerLine();
	quint16 colour;
	if (format() == "map") {
		for(quint8 y = 0; y < 22; y++)
			for(quint8 x = 21-y; x<=22+y; x++) {
				stream >> colour;
				d[y*bytesPerLine/sizeof(QRgb)+x] = qRgb((colour>>7)&0xF8,(colour>>2)&0xF8,(colour<<3)&0xF8);
			}
		for(quint8 y = 22; y < mHeight; y++)
			for(quint8 x = y-22; x<66-y; x++) {
				stream >> colour;
				d[y*bytesPerLine/sizeof(QRgb)+x] = qRgb((colour>>7)&0xF8,(colour>>2)&0xF8,(colour<<3)&0xF8);
			}
	} else {
		QVector<quint16> lookupTable(mHeight);
		for(QVector<quint16>::iterator iter = lookupTable.begin();iter!=lookupTable.end(); iter++)
			stream >> *iter;
		qint64 start = stream.device()->pos();
		for (quint16 y = 0; y < mHeight; y++) {
			quint16 x = 0;
			if(!stream.device()->seek(start + (lookupTable[y] << 1)))
				return false;
			quint16 xoffset, rlength;
			do {
				stream >> xoffset >> rlength;
				x += xoffset;
				if (xoffset + rlength >= 2048)
					break;
				for (quint16 c = rlength; c > 0; c--, x++) {
					stream >> colour;
					d[y*bytesPerLine/sizeof(QRgb)+x] = qRgb((colour>>7)&0xF8,(colour>>2)&0xF8,(colour<<3)&0xF8);
				}
			} while ((xoffset + rlength) != 0);
		}
	}
	return true;
}

bool ArtIOHandler::supportsOption(QImageIOHandler::ImageOption option) const {
	return option == QImageIOHandler::Size || ImageIOHandler::supportsOption(option);
}

ArtIOHandler::ArtIOHandler(QIODevice* device, const QByteArray& format) :
	ImageIOHandler(device,format), mFlags(-1), mWidth(0), mHeight(0) {
}

ArtIOHandler::~ArtIOHandler() {
}

QVariant ArtIOHandler::option(QImageIOHandler::ImageOption option) const {
	switch (option) {
	case QImageIOHandler::Size:
		return mWidth == 0 && mHeight == 0 ? QVariant()
				: QSize(mWidth, mHeight);
	default:
		return ImageIOHandler::option(option);
	}
}

// TODO: Implement animdata.mul handling!

//// prepare datastream
//Q_ASSERT_X(QFile::exists(_animFile), __PRETTY_FUNCTION__, "Couldn't find animData!");
//mAnimStream.setDevice(new QFile(_animFile));
//Q_ASSERT(mAnimStream.device()->open(QIODevice::ReadOnly));
//mAnimStream.setByteOrder(QDataStream::LittleEndian);
//
//// process animdata.mul
//quint32 i = 0;
//while(!mAnimStream.atEnd()) {
//	if( i%8==0 )
//		mAnimStream.skipRawData(4); // skip 4 byte header
//	qint64 offset = mAnimStream.device()->pos();
//	mAnimStream.skipRawData(65); // skip 64 byte frameIDs + 1 byte unknown
//	quint8 frameCount = 0;
//	mAnimStream >> frameCount;
//	mAnimStream.skipRawData(2); // skip 1 byte frameDelay + 1 byte startDelay
//	if(frameCount>0&&frameCount<=64)
//		mAnimDataOffsets[i] = offset;
//	i++;
//}
//}
//
//Arts::Animation Arts::getAnimData( ID _id ) {
//Arts::Animation result;
//mAnimStream.device()->seek( mAnimDataOffsets[_id] );
//result.mIDs.resize(64);
//for( quint8 i = 0; i < 64; i ++ ) {
//	qint8 frame = 0;
//	mAnimStream >> frame;
//	result.mIDs[i] = _id + frame;
//}
//mAnimStream.skipRawData(1);
//quint8 frameCount;
//mAnimStream >> frameCount;
//result.mIDs.resize(frameCount);
//mAnimStream >> result.mFrameDelay >> result.mStartDelay;
//return result;
//}
