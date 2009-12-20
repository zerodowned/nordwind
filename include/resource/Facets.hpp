#ifndef CMAPS_H_
#define CMAPS_H_

#include "IndexFile.hpp"

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
			typedef QVector<MapTile> MapTiles;
			typedef QVector<StaticTile> StaticTiles;
			class Block {
				public:
					Block( const QPoint& _offset, const MapTiles& _mapTiles, const StaticTiles& _staticTiles );
					QPoint getOffset() const;
					MapTiles getMapTiles() const;
					StaticTiles getStaticTiles() const;
				private:
					QPoint mOffset;
					MapTiles mMapTiles;
					StaticTiles mStaticTiles;
			};
			class Entry : public IndexFile {
				public:
					Entry( QString _name, QString _mapFile, QSize _dimension, QString _indexFile, QString _dataFile, QObject* _parent, QSize _blockSize = QSize(8,8));
					virtual ~Entry();
					QSharedPointer<Block> getBlock( const QPoint& _blockPos );
					bool isSameBlock( const QPoint& _posA, const QPoint& _posB ) const;
					QSize getSize() const;
					QString getName() const;
					ID getBlockID( QPoint _pos ) const;
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
	: mOffset(_offset),
	  mMapTiles(_mapTiles),
	  mStaticTiles(_staticTiles) {
	}

	inline QPoint Facets::Block::getOffset() const {
		return mOffset;
	}

	inline Facets::MapTiles Facets::Block::getMapTiles() const {
		return mMapTiles;
	}

	inline Facets::StaticTiles Facets::Block::getStaticTiles() const {
		return mStaticTiles;
	}

	inline bool Facets::Entry::isSameBlock( const QPoint& _posA, const QPoint& _posB ) const {
		return (getBlockID(_posA)==getBlockID(_posB)) ? true : false;
	}

	inline ID Facets::Entry::getBlockID( QPoint _pos ) const {
		return convertBlockPosToID( QPoint(_pos.x()/mBlockSize.width(), _pos.y()/mBlockSize.height()) );
	}

	inline ID Facets::Entry::convertBlockPosToID( const QPoint& _blockPos ) const {
		return (_blockPos.x()*mSize.height()) + _blockPos.y();
	}

	inline QSize Facets::Entry::getSize() const {
		return mSize;
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
