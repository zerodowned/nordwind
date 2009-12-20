/*
 * TileInfoLand.cpp
 *
 *  Created on: 13.12.2009
 *      Author: idstein
 */

#include "resource/TileData.hpp"
#include <qfile.h>
#include <qmetaobject.h>

using namespace resource;

TileData::LandInfo::operator QString() const {
	QMetaEnum flag = TileData::staticMetaObject.enumerator(TileData::staticMetaObject.indexOfEnumerator("Flags"));
	return QString("Name:%1\n"
				   "TextureID:%2\n"
                                   "Flags:%3\n")
				   .arg(mName)
				   .arg(mTextureID, 4, 16)
				   .arg(mFlags, 0, 16)
				    + flag.valueToKeys(mFlags);
}

TileData::TileInfo::operator QString() const {
	QMetaEnum flag = TileData::staticMetaObject.enumerator(TileData::staticMetaObject.indexOfEnumerator("Flags"));
	return QString("Name:%1\n"
			"Weight:%2\n"
			"Quality:%3\n"
			"Hitpoints:%4\n"
			"Unknown1:%5\n"
			"Quantity:%6\n"
			"Animation:%7\n"
			"Unknown2:%8\n"
			"Hue:%9\n"
			"StackOffset:%10\n"
			"Unknown3:%11\n"
			"Height:%12\n"
                        "Flags:%13\n")
			.arg(mName)
			.arg(mWeight)
			.arg(mQuality)
			.arg(mHitpoints)
			.arg(mUnknown1, 2, 16)
			.arg(mQuantity)
			.arg(mAnimation, 4, 16)
			.arg(mUnknown2, 2, 16)
			.arg(mHue, 2, 16)
			.arg(mStackOffset)
			.arg(mUnknown3, 2, 16)
			.arg(mHeight)
			.arg(mFlags,0,16)
			 + flag.valueToKeys(mFlags);
}


TileData::TileData(QString _fileName, QObject* _parent)
: QObject(_parent) {
	QSharedPointer<QFile> file( new QFile(_fileName) );
	QDataStream stream( file.data() );
	stream.setByteOrder(QDataStream::LittleEndian);
	Q_ASSERT_X(file->open(QIODevice::ReadOnly),__PRETTY_FUNCTION__,_fileName.toAscii().constData());

	quint16 i = 0;
	mLandInfos.resize(0x4000);
	while( i < 0x4000 ) {
		if(i%32==0)
			stream.skipRawData(4); // skip 4 byte header;
		int flags;
		stream >> flags
			   >> mLandInfos[i].mTextureID;
		mLandInfos[i].mFlags = TileData::Flags( flags );
		char tmp[20];
		stream.readRawData( tmp, 20 );
		mLandInfos[i].mName = tmp;
		i++;
	}
	i = 0;
	mTileInfos.resize( ((stream.device()->size()-stream.device()->pos())/1188)<<5 );
	while( !stream.atEnd() ) {
		if( i%32 == 0 )
			stream.skipRawData(4); // skip 4 byte header;
		int flags;
		stream >> flags
			   >> mTileInfos[i].mWeight
			   >> mTileInfos[i].mQuality
			   >> mTileInfos[i].mHitpoints
			   >> mTileInfos[i].mUnknown1
			   >> mTileInfos[i].mQuantity
			   >> mTileInfos[i].mAnimation
			   >> mTileInfos[i].mUnknown2
			   >> mTileInfos[i].mHue
			   >> mTileInfos[i].mStackOffset
			   >> mTileInfos[i].mUnknown3
			   >> mTileInfos[i].mHeight;
		mTileInfos[i].mFlags = TileData::Flags( flags );
		char tmp[20];
		stream.readRawData( tmp, 20 );
		mTileInfos[i].mName = tmp;
		i++;
	}
}

TileData::~TileData() {
}
