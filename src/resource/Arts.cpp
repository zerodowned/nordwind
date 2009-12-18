#include "resource/Arts.hpp"
#include "resource/Cache.hpp"
#include <qfile.h>

using namespace resource;

Arts::Arts( QString _animFile, QString _indexFile, QString _dataFile, QObject* _parent )
: IndexFile(_indexFile,_dataFile,_parent) {
	// prepare datastream
	Q_ASSERT_X(QFile::exists(_animFile), __PRETTY_FUNCTION__, "Couldn't find animData!");
	mAnimStream.setDevice(new QFile(_animFile));
	Q_ASSERT(mAnimStream.device()->open(QIODevice::ReadOnly));
	mAnimStream.setByteOrder(QDataStream::LittleEndian);

	// process animdata.mul
	quint32 i = 0;
	while(!mAnimStream.atEnd()) {
		if( i%8==0 )
			mAnimStream.skipRawData(4); // skip 4 byte header
		qint64 offset = mAnimStream.device()->pos();
		mAnimStream.skipRawData(65); // skip 64 byte frameIDs + 1 byte unknown
		quint8 frameCount = 0;
		mAnimStream >> frameCount;
		mAnimStream.skipRawData(2); // skip 1 byte frameDelay + 1 byte startDelay
		if(frameCount>0&&frameCount<=64)
			mAnimDataOffsets[i] = offset;
		i++;
	}
}

Arts::~Arts() {
}

QSharedPointer<Arts::Animation> Arts::getAnimData( ID _id ) {
	QSharedPointer<Animation> result( new Animation );
	mAnimStream.device()->seek( mAnimDataOffsets[_id] );
	result->mIDs.resize(64);
	for( quint8 i = 0; i < 64; i ++ ) {
		qint8 frame = 0;
		mAnimStream >> frame;
		result->mIDs[i] = _id + frame;
	}
	mAnimStream.skipRawData(1);
	quint8 frameCount;
	mAnimStream >> frameCount;
	result->mIDs.resize(frameCount);
	mAnimStream >> result->mFrameDelay >> result->mStartDelay;
	return result;
}

QSharedPointer<Arts::Entry> Arts::getArtStatic( ID _id, Hue _hue ) {
	QByteArray key = QByteArray::number(Object::Arts) + QByteArray::number(_id) + QByteArray::number(_hue->getID());
	QSharedPointer<Arts::Entry> result = Cache::instance().lookup<Arts::Entry>(key);
	if(result.isNull()) {
		QByteArray data = getData(_id);
		if(!data.isEmpty()) {
			if(mAnimDataOffsets.contains(_id)) {
				QSharedPointer<Animation> animData = getAnimData( _id );
				QVector< QSharedPointer<Entry> > frames;
				foreach( ID id, animData->mIDs ) {
					if( id != _id ) {
						frames.push_back( getArtStatic(id, _hue) );
					} else {
						frames.push_back( QSharedPointer<Entry>() );
					}
				}
				result = Cache::instance().manage<Arts::AnimationEntry>( new Arts::AnimationEntry(_id,decodeStatic(data, _hue),_hue, frames, animData));
			} else {
				result = Cache::instance().manage<Arts::Entry>( new Arts::Entry( _id, decodeStatic( data, _hue ), _hue ) );
			}
		}
	}
	return result;
}

QSharedPointer<Arts::Entry> Arts::getArtLand( ID _id, Hue _hue ) {
	QByteArray key = QByteArray::number(Object::Arts) + QByteArray::number(_id) + QByteArray::number(_hue->getID());
	QSharedPointer<Arts::Entry> result = Cache::instance().lookup<Arts::Entry>(key);
	if(result.isNull()) {
		QByteArray data = getData(_id);
		if(!data.isEmpty()) {
			result = Cache::instance().manage<Arts::Entry>( new Arts::Entry( _id, decodeLand( data, _hue ), _hue ) );
		}
	}
	return result;
}

QSharedPointer<Arts::Entry> Arts::getEntry( ID _id, QSharedPointer<Hues::Entry> _hue ) {
	return (_id<0x4000) ? getArtLand(_id,_hue) : getArtStatic(_id,_hue);
}



Image Arts::decodeStatic( QByteArray _data, QSharedPointer<Hues::Entry> _hue ) {
	QDataStream stream(_data);
	stream.setByteOrder(QDataStream::LittleEndian);

	quint32 flag;
	stream >> flag;
	// RAW Tile check
	if(flag>0xFFFF || flag == 0)
		return decodeLand(_data,_hue);

	quint16 width, height;
	stream >> width >> height;
	QImage image(width,height,QImage::Format_ARGB32);
	image.fill( 0x00000000 );

	QVector<quint16> lookupTable( height, 0x0000 );
	stream.readRawData( (char*)lookupTable.data(), sizeof(quint16)*height); // Read in the offset table for every row

	quint16 y = 0;
	foreach( quint16 offset, lookupTable ) {
		quint16 x = 0;
		stream.device()->seek(offset<<1);
		quint16 xoffset, rlength;
		do {
			stream >> xoffset >> rlength;
			x += xoffset;
			for( quint16 dx = 0; dx < rlength; dx++,x++ ) {
				Colour16 colour16;
				stream >> colour16;
				if( colour16 == 0)
					continue;
				image.setPixel( x, y, _hue->mapToHue(colour16) );
			}
		} while((xoffset+rlength)!=0);
		y++;
	}
	return Image(new QPixmap(QPixmap::fromImage(image)));
}

Image Arts::decodeLand( QByteArray _data, QSharedPointer<Hues::Entry> _hue ) {
	QDataStream stream(_data);
	stream.setByteOrder(QDataStream::LittleEndian);

	quint32 flag;
	stream >> flag;
	// RAW Tile check
//	if(flag<0xFFFF && flag > 0)
//		return decodeStatic(_data,_hue);

	QImage image(44,44,QImage::Format_ARGB32);
	image.fill( 0x00000000 );

	QVector<Colour16> imgData( 1024 );
	stream.readRawData( (char*)imgData.data(), sizeof(Colour16)<<10 );

	quint16 coffset = 0;
        quint8 x, y;
        for( y = 0; y < 22; y++ ) {
                for( x = 21 - y; x < (23+y); x++, coffset++ ) {
			image.setPixel( x,y, _hue->mapToHue(imgData[coffset]));
		}
	}
        for( ; y < 44; y++ ) {
                for( x = y-22; x < (66-y); x++, coffset++ ) {
			image.setPixel( x,y, _hue->mapToHue(imgData[coffset]));
		}
	}
	return Image(new QPixmap(QPixmap::fromImage(image)));
}



//QImage* CArts::getCursor(qint32& _hotX, qint32& _hotY, quint32 _id, quint32 _hue, bool _partialhue ) {
//	quint32 offset = 0xFFFFFFFF; // Default to invalid
//	quint32 length = 0xFFFFFFFF; // Length of data record
//	QVector<Colour> *hue = Resources::instance().hues()->getHue(_hue);
//
//	// first 0x4000 are land arts
//	_id += 0x4000; // Static items start at id 0x4000
//
//	//stVerdataRecord *patch = Verdata->getPatch(VERDATA_ART, id);
//	//if (patch) {
//	//	dataStream.setDevice(Verdata->getIoDevice());
//	//	offset = patch->offset;
//	//	length = patch->length;
//	//} else {
//		m_indexStream.device()->seek(12 * _id);
//		m_indexStream >> offset >> length; // Read index data
//	//}
//
//	// Sanity checks
//	if (offset != 0xFFFFFFFF && length > 0 && length != 0xFFFFFFFF) {
//        m_dataStream.device()->seek(offset);
//
//		quint32 header;
//		quint16 width, height;
//		m_dataStream >> header >> width >> height;
//		// Create surface and set color key
//		QImage* img = new QImage( width-1, height-1, QImage::Format_ARGB32 );
//		Q_CHECK_PTR(img);
//		img->fill( 0x00000000 );
//
//		QVector<quint16> lookupTable(height);
//
//		m_dataStream.readRawData( (char*)(lookupTable.data()), sizeof(quint16)*height); // Read in the offset table for every row
//
//		offset = m_dataStream.device()->pos(); // Save offset
//
//		for (quint16 y = 0; y < height; ++y) {
//			m_dataStream.device()->seek(offset + lookupTable[y] * 2);
//			quint16 x = 0;
//
//			while ((quint32)m_dataStream.device()->pos() < offset + length) {
//				quint16 xoffset, rlength;
//				m_dataStream >> xoffset >> rlength;
//
//				if( !xoffset && !rlength )
//					break;
//				quint16 xend = x + xoffset + rlength;
//				for( x = x + xoffset; x < xend; ++x ) {
//					quint16 color;
//					m_dataStream >> color;
//					if ( color == 0 ) {
//						continue; // Skip transparent pixels
//					}
//					quint8 index = (color >> 10) & 0x1F;
//					quint8 r = index << 3; // Multiply with 8
//					quint8 g = (color >> 2) & 0xF8;
//					quint8 b = (color << 3) & 0xF8;
//					if( x == 0 || y == 0 || x == width-1 || y == height-1 ) {
//						if( y==0 && g == 0xF8 )
//							_hotX = x-1;
//						if( x==0 && g == 0xF8 )
//							_hotY = y-1;
//						continue;
//					}
//					if( hue ) {
//						if(!_partialhue || r==g && r==b ) {
//							img->setPixel( x-1,y-1, hue->at( index ) );
//						} else {
//							img->setPixel( x-1, y-1, qRgb( r, g, b ) );
//						}
//					} else {
//						img->setPixel( x-1, y-1, qRgb( r, g, b ) );
//					}
//				}
//			}
//		}
//		//QPixmap* pix = &QPixmap::fromImage( *img );
//		//Q_CHECK_PTR(pix);
//		//delete img;
//		return img;
//	}
//
//	return NULL;
//}
