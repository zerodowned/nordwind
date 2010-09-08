#include "ArtIOHandler.hpp"
#include <qdatastream.h>
#include <qimage.h>
#include <qvariant.h>
#include <qdebug.h>

bool ArtIOHandler::canRead(QIODevice* device, QByteArray* format) {
	quint32 flags;
	if(device->peek((char*)(&flags),4)!=4)
		return false;
	*format = flags == 0xFFFF || flags == 0 ? "Tile" : "Static";
	return true;
}

bool ArtIOHandler::canRead() const {
	if (!canRead(q_ptr->device(), &mSubType))
		return false;
	q_ptr->setFormat(mSubType);
	return true;
}

bool ArtIOHandler::read(QImage *image) {
	qDebug() << "Constructing art";
	QDataStream stream(q_ptr->device());
	stream.setByteOrder(QDataStream::LittleEndian);
	if (q_ptr->format() == "Tile")
		mWidth = mHeight = 44;
	else
		stream >> mWidth >> mHeight;
	*image = QImage(mWidth, mHeight, QImage::Format_RGB555);
	if (image->isNull())
		return false;
	if (mSubType == "Tile") {
		QScopedArrayPointer<quint16> raw(new quint16[mWidth * mHeight]);
		if (stream.readRawData((char*) raw.data(), mWidth * mHeight) != mWidth
				* mHeight)
			return false;
		quint8 x, y;
		quint16 coffset = 0;
		for (y = 0; y < 22; y++)
			for (x = 21 - y; x < (23 + y); x++, coffset++)
				image->setPixel(x, y, raw[coffset]);
		for (; y < 44; y++)
			for (x = y - 22; x < (66 - y); x++, coffset++)
				image->setPixel(x, y, raw[coffset]);
	} else {
		QScopedArrayPointer<quint16> lookupTable(new quint16[mHeight]);
		stream.readRawData((char*) lookupTable.data(), mHeight);
		qint64 start = stream.device()->pos();
		for (quint16 y = 0; y < mHeight; y++) {
			quint16 x = 0;
			stream.device()->seek(start + (lookupTable[y] << 1));
			quint16 xoffset, rlength;
			do {
				stream >> xoffset >> rlength;
				x += xoffset;
				if (xoffset + rlength >= 2048)
					break;
				for (quint16 c = rlength; c > 0; c--, x++) {
					quint16 colour16;
					stream >> colour16;
					image->setPixel(x, y, colour16);
				}
			} while ((xoffset + rlength) != 0);
		}
	}
	return true;
}

bool ArtIOHandler::supportsOption(QImageIOHandler::ImageOption option) const {
	return option == QImageIOHandler::SubType || option
			== QImageIOHandler::Size || OSIImageIOHandlerPrivate::supportsOption(option);
}

ArtIOHandler::ArtIOHandler(OSIImageIOHandler *q) :
	OSIImageIOHandlerPrivate(q), mFlags(-1), mWidth(0), mHeight(0) {
}

ArtIOHandler::~ArtIOHandler() {
}

QVariant ArtIOHandler::option(QImageIOHandler::ImageOption option) const {
	switch (option) {
	case QImageIOHandler::SubType:
		return mSubType.isNull() ? QVariant() : mSubType;
	case QImageIOHandler::Size:
		return mWidth == 0 && mHeight == 0 ? QVariant()
				: QSize(mWidth, mHeight);
	default:
		return OSIImageIOHandlerPrivate::option(option);
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
