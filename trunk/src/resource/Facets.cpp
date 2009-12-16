#include "resource/Facets.hpp"
#include <qfile.h>

using namespace resource;

Facets::Entry::Entry(QString _name, QString _mapFile, QSize _dimension, QString _indexFile, QString _dataFile, QObject* _parent )
: IndexFile(_indexFile, _dataFile, _parent),
  mName(_name),
  mDimension(_dimension) {
	mMapStream.setDevice( new QFile(_mapFile, this) );
	Q_ASSERT_X(mMapStream.device()->open(QIODevice::ReadOnly),__PRETTY_FUNCTION__,_mapFile.toAscii().constData());
	mMapStream.setByteOrder(QDataStream::LittleEndian);
}

Facets::Entry::~Entry() {
}

QSharedPointer<Facets::Block> Facets::Entry::getBlock( const QPoint& _blockPos ) {
	ID id = getBlockID(_blockPos);
	if(!mBlockCache.contains(id)) {
		ID blockID = getBlockID(_blockPos);
		Facets::Block* block = new Facets::Block( decodeMap( getMapData(blockID) ), decodeStatic( getData(blockID) ) );
		mBlockCache.insert( id, new QSharedPointer<Facets::Block>( block ) );
	}
	return QSharedPointer<Facets::Block>( *(mBlockCache[id]) );
}

QByteArray Facets::Entry::getMapData( ID _id ) {
	Q_ASSERT(mMapStream.device()->seek( _id * 196 ));
	QByteArray result( 196, 0x00 );
	mMapStream.readRawData(result.data(), 196);
	return result;
}

Facets::MapTiles Facets::Entry::decodeMap( QByteArray _data ) {
	QDataStream stream(_data);
	stream.setByteOrder(QDataStream::LittleEndian);
	stream.skipRawData(4); //skip header
	MapTiles result;
	for( quint8 i = 0; i < 64; i ++ ) {
		MapTile mapTile;
		stream >> mapTile.mID >> mapTile.mZ;
		result.push_back(mapTile);
	}
	return result;
}

Facets::StaticTiles Facets::Entry::decodeStatic( QByteArray _data ) {
	QDataStream stream(_data);
	stream.setByteOrder(QDataStream::LittleEndian);
	StaticTiles result( _data.size()/7 );
	foreach( StaticTile staticTile, result ) {
		stream >> staticTile.mID
			   >> staticTile.mXOffset
			   >> staticTile.mYOffset
			   >> staticTile.mZ
			   >> staticTile.mHueID;
	}
	return result;
}
