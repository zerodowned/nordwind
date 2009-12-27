#include "resource/Facets.hpp"
#include <qfile.h>

using namespace resource;

Facets::Entry::Entry(QString _name, QString _mapFile, QSize _dimension, QString _indexFile, QString _dataFile, QObject* _parent, QSize _blockSize )
: IndexFile(_indexFile, _dataFile, _parent),
  mName(_name),
  mSize(_dimension),
  mBlockSize(_blockSize) {
	mMapStream.setDevice( new QFile(_mapFile, this) );
	Q_ASSERT_X(mMapStream.device()->open(QIODevice::ReadOnly),__PRETTY_FUNCTION__,_mapFile.toAscii().constData());
	mMapStream.setByteOrder(QDataStream::LittleEndian);
}

Facets::Entry::~Entry() {
}

Facets::Block Facets::Entry::getBlock( const QPoint& _blockPos ) {
	ID id = convertBlockPosToID(_blockPos);

	if(!mCache.contains(id)) {
		MapTiles mapTiles = decodeMap( getMapData(id) );
		if(!mapTiles.isEmpty()) {
			MapTiles southTiles = decodeMap( getMapData(convertBlockPosToID(_blockPos+QPoint(0,1))) );
			for( qint32 x = 0; x < mBlockSize.width(); x++ ) {
				mapTiles.insert(QPoint(x,mBlockSize.height()), !southTiles.isEmpty() ? southTiles[QPoint(x,0)] : mapTiles[QPoint(x,mBlockSize.height()-1)] );
			}

			MapTiles eastTiles = decodeMap( getMapData(convertBlockPosToID(_blockPos+QPoint(1,0))) );
			for( qint32 y = 0; y < mBlockSize.height(); y++ ) {
				mapTiles.insert(QPoint(mBlockSize.width(),y), !eastTiles.empty() ? eastTiles[QPoint(0,y)] : mapTiles[QPoint(mBlockSize.width()-1,y)]  );
			}

			MapTiles downTiles = decodeMap( getMapData(convertBlockPosToID(_blockPos+QPoint(1,1))) );
			mapTiles.insert(QPoint(mBlockSize.width(),mBlockSize.height()), !downTiles.isEmpty() ? downTiles[QPoint(0,0)] : mapTiles[QPoint(mBlockSize.width()-1,mBlockSize.height()-1)] );
		}

		Facets::Block block(QPoint(_blockPos.x()*mBlockSize.width(),_blockPos.y()*mBlockSize.height()),
													mapTiles,
													decodeStatic( getData(id) ) );
		mCache.insert(id, new Block(block));
		return block;
	}
	return Block(*(mCache[id]));
}

QByteArray Facets::Entry::getMapData( ID _id ) {
	quint32 mapDataSize = 4 + 3*mBlockSize.width()*mBlockSize.height();
	Q_ASSERT(mMapStream.device()->seek( _id * mapDataSize ));
	QByteArray result( mapDataSize, 0x00 );
	mMapStream.readRawData(result.data(), mapDataSize);
	return result;
}

Facets::MapTiles Facets::Entry::decodeMap( QByteArray _data ) {
	MapTiles result;
	if(!_data.isEmpty()) {
		QDataStream stream(_data);
		stream.setByteOrder(QDataStream::LittleEndian);
		stream.skipRawData(4); //skip header
		for( quint8 i = 0; !stream.atEnd(); i++ ) {
			MapTile mapTile;
			stream >> mapTile.mID >> mapTile.mZ;
			result.insert( QPoint(i%mBlockSize.width(),i/mBlockSize.width()),mapTile);
		}
		if(result.size()!=mBlockSize.width()*mBlockSize.height())
			result.clear();
	}
	return result;
}

Facets::StaticTiles Facets::Entry::decodeStatic( QByteArray _data ) {
	StaticTiles result;
	if(!_data.isEmpty()) {
		QDataStream stream(_data);
		stream.setByteOrder(QDataStream::LittleEndian);
		while(!stream.atEnd()) {
			StaticTile staticTile;
			stream >> staticTile.mID
			>> staticTile.mXOffset
			>> staticTile.mYOffset
			>> staticTile.mZ
			>> staticTile.mHueID;
			result.push_back(staticTile);
		}
	}
	return result;
}
