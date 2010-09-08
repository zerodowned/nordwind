/*
 * TileInfoLand.cpp
 *
 *  Created on: 13.12.2009
 *      Author: idstein
 */

#include "TileData.hpp"
#include <qfile.h>
#include <qdebug.h>

using namespace resource;

Info::Flags Info::flags() const {
	return mFlags;
}

quint16 Info::textureID() const {
	return mGeneric;
}

QString Info::name() const {
	return mName;
}

quint8 Info::weight() const {
	return mGeneric >> 16;
}

quint8 Info::quality() const {
	return mGeneric & 0xFF;
}

quint16 Info::hitpoints() const {
	return mHitpoints;
}

quint8 Info::quantity() const {
	return mQuantity;
}

quint16 Info::animation() const {
	return mAnimation;
}

quint8 Info::hue() const {
	return mHue;
}

quint8 Info::stackOffset() const {
	return mStackOffset;
}

quint8 Info::value() const {
	return mValue;
}

quint8 Info::height() const {
	return mHeight;
}

bool Info::hasFullInfo() const {
	return mFullInfo;
}

TileData::TileData(QString fileName) {
	QFile file(fileName);
	Q_ASSERT_X(file.open(QIODevice::ReadOnly), __PRETTY_FUNCTION__,
			fileName.toAscii().constData());
	QDataStream stream(file.readAll());
	stream.setByteOrder(QDataStream::LittleEndian);
        qDebug() << "TileData" << stream.device()->size() << "Bytes";
        quint32 fileSize = stream.device()->size() - 512*(32*26+4);
        // 16384 land infos + bytes left / block size * 32 elements per block
        resize(16384+ ((stream.device()->size() - 512*(32*26+4)) / (32*37+4))
			* 32);

        for (quint16 i = 0; i < size(); i++) {
		if (i % 32 == 0)
			stream.skipRawData(4); // skip 4 byte header;
		Info& info = const_cast<Info&>(at(i));
		int flags;
		stream >> flags >> info.mGeneric;
		if (i > 0x4000) {
			info.mFullInfo = true;
			stream >> info.mHitpoints
			>> info.mUnknown1 >> info.mQuantity >> info.mAnimation
			>> info.mUnknown2 >> info.mHue >> info.mStackOffset
			>> info.mValue >> info.mHeight;
		} else {
			info.mFullInfo = false;
		}
		char tmp[20];
		stream.readRawData(tmp, 20);
		info.mFlags = Info::Flags(flags);
		info.mName = tmp;
	}
}

TileData::~TileData() {
}
