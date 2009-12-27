#ifndef CMAPS_H_
#define CMAPS_H_

#include "IndexFile.hpp"
#include <qcache.h>
#include <qmap.h>

namespace resource {

	class Facets : public QObject {
		public:
			struct MapTile {
				quint16 mID;
				qint8 mZ;
			};
			struct StaticTile {
				quint16 mID;
				quint8 mXOffset;
				quint8 mYOffset;
				qint8 mZ;
				quint16 mHueID;
			};
			typedef QMap<QPoint, MapTile> MapTiles;
			typedef QList<StaticTile> StaticTiles;
			class Block {
				public:
					Block( const QPoint& _offset, const MapTiles& _mapTiles, const StaticTiles& _staticTiles );
					Block( const Block& _o );
					QPoint getOffset() const;
					MapTiles getMapTiles() const;
					StaticTiles getStaticTiles() const;
					bool isValid() const;
				private:
					class Private : public QSharedData {
						public:
							QPoint mOffset;
							// MapCells + 1 row of south Block + first Cell of Down Block + 1 column of east Block
							/* +----------------+-+
							 * |				| |
							 * |				|1|
							 * |				|x|
							 * |	  8 x 8		|8|
							 * |				| |
							 * |				| |
							 * +----------------+-+
							 * |	8x1			| | 1x1
							 * +----------------+-+
							 */
							MapTiles mMapTiles;
							StaticTiles mStaticTiles;
					};
					QSharedDataPointer<Private> mData;
			};
			class Entry : public IndexFile {
				public:
					Entry( QString _name, QString _mapFile, QSize _dimension, QString _indexFile, QString _dataFile, QObject* _parent, QSize _blockSize = QSize(8,8));
					virtual ~Entry();
					Block getBlock( const QPoint& _blockPos );
					bool isSameBlock( const QPoint& _posA, const QPoint& _posB ) const;
					QSize getSize() const;
					QSize getBlockSize() const;
					QString getName() const;
					ID getBlockID( QPoint _pos ) const;
					QPoint fromCellToBlockPosition( QPoint _cell ) const;
				private:
					ID convertBlockPosToID( const QPoint& _pos ) const;
					QByteArray getMapData( ID _id );
					MapTiles decodeMap( QByteArray _data );
					StaticTiles decodeStatic( QByteArray _data );
					QString mName;
					QSize mSize; // dimension counted in blocks!
					QSize mBlockSize;
					QDataStream mMapStream;
					QCache<ID,Block> mCache;
			};
			Facets( QObject* _parent );
			virtual ~Facets();
			Facets& addFacet( QString _name, QString _mapFile, QSize _dimension, QString _staticsIndex, QString _staticsData );
			QSharedPointer<Facets::Entry> getFacet( QString _name );
		private:
			QHash<QString, QSharedPointer<Entry> > mFacets;
	};

	typedef QSharedPointer<Facets::Entry> Facet;
	typedef Facets::Block Block;

	inline Facets::Block::Block(const QPoint& _offset, const Facets::MapTiles& _mapTiles, const Facets::StaticTiles& _staticTiles )
	: mData(new Facets::Block::Private) {
		mData->mOffset = _offset;
		mData->mMapTiles = _mapTiles;
		mData->mStaticTiles = _staticTiles;
	}

	inline Facets::Block::Block(const Facets::Block& _o)
	: mData(_o.mData) {
	}

	inline QPoint Facets::Block::getOffset() const {
		return mData->mOffset;
	}

	inline Facets::MapTiles Facets::Block::getMapTiles() const {
		return mData->mMapTiles;
	}

	inline Facets::StaticTiles Facets::Block::getStaticTiles() const {
		return mData->mStaticTiles;
	}

	inline bool Facets::Block::isValid() const {
		return !mData->mMapTiles.isEmpty() ? true : false; // TODO add BlockSize check
	}

	inline bool Facets::Entry::isSameBlock( const QPoint& _posA, const QPoint& _posB ) const {
		return (getBlockID(_posA)==getBlockID(_posB)) ? true : false;
	}

	inline ID Facets::Entry::getBlockID( QPoint _pos ) const {
		return convertBlockPosToID( fromCellToBlockPosition(_pos) );
	}

	inline QPoint Facets::Entry::fromCellToBlockPosition( QPoint _cell ) const {
		return QPoint(_cell.x()/mBlockSize.width(),_cell.y()/mBlockSize.height());
	}

	inline ID Facets::Entry::convertBlockPosToID( const QPoint& _blockPos ) const {
		return (_blockPos.x()*mSize.height()) + _blockPos.y();
	}

	inline QSize Facets::Entry::getSize() const {
		return mSize;
	}

	inline QSize Facets::Entry::getBlockSize() const {
		return mBlockSize;
	}

	inline QString Facets::Entry::getName() const {
		return mName;
	}

	inline Facets::Facets( QObject* _parent )
	: QObject(_parent) {
	}

	inline Facets::~Facets() {
	}

	inline Facets& Facets::addFacet( QString _name, QString _mapFile, QSize _dimension, QString _staticsIndex, QString _staticsData ) {
		mFacets[_name] = Facet( new Facets::Entry(_name, _mapFile, _dimension, _staticsIndex, _staticsData, this ) );
		return *this;
	}

	inline Facet Facets::getFacet( QString _name ) {
		return mFacets.value(_name);
	}
}
#endif
