/*
 * TileInfoLand.cpp
 *
 *  Created on: 13.12.2009
 *      Author: idstein
 */

#include "resource/TileData.hpp"
#include <qfile.h>

using namespace resource;

TileData::TileData(QString _fileName, QObject* _parent)
: QObject(_parent) {
	QSharedPointer<QFile> file( new QFile(_fileName) );
	QDataStream stream( file.data() );
	stream.setByteOrder(QDataStream::LittleEndian);
	Q_ASSERT_X(file->open(QIODevice::ReadOnly),__PRETTY_FUNCTION__,_fileName.toAscii().constData());

	quint16 i = 0;
	while( i < 0x4000 ) {
		stream.skipRawData(4); // skip 4 byte header;
		for(quint8 c = 0; c < 32; c++, i++ ) {
			QSharedPointer<TileData::LandInfo> entry( new TileData::LandInfo );
			int flags;
			stream >> flags
				   >> entry->mTextureID;
			entry->mFlags = TileData::Flags( flags );
			char tmp[20];
			stream.readRawData( tmp, 20 );
			entry->mName = tmp;
			mEntries.insert( i, entry );
		}
	}
	while( !stream.atEnd() ) {
		if( i%32 == 0 )
			stream.skipRawData(4); // skip 4 byte header;
		QSharedPointer<TileData::TileInfo> entry( new TileData::TileInfo );
		int flags;
		stream >> flags
			   >> entry->mWeight
			   >> entry->mQuality
			   >> entry->mHitpoints
			   >> entry->mUnknown1
			   >> entry->mQuantity
			   >> entry->mAnimation
			   >> entry->mUnknown2
			   >> entry->mHue
			   >> entry->mStackOffset
			   >> entry->mUnknown3
			   >> entry->mHeight;
		entry->mFlags = TileData::Flags( flags );
		char tmp[20];
		stream.readRawData( tmp, 20 );
		entry->mName = tmp;
		mEntries.insert( i, entry );
                i++;
	}
}

TileData::~TileData() {
}
