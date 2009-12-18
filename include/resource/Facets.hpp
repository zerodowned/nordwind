#ifndef CMAPS_H_
#define CMAPS_H_

#include "IndexFile.hpp"
#include "Object.hpp"
#include <qcache.h>
#include <qmap.h>
#include <qpointer.h>

namespace resource {

	class Facets : public QObject {
		public:
			struct MapTile {
				quint16 mID;
				quint8 mZ;
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
			class Entry;
			class Block : public QPair<MapTiles,StaticTiles>, public Object {
				public:
					Block( QPointer<Entry> _facet, QPoint _offset, MapTiles _mapTiles, StaticTiles _staticTiles );
					virtual ~Block();
					QPoint getOffset() const;
					QPointer<Entry> getFacet() const;
				private:
					QPointer<Entry> mFacet;
					QPoint mOffset;
			};
			class Entry : public IndexFile {
				public:
					Entry( QString _name, QString _mapFile, QSize _dimension, QString _indexFile, QString _dataFile, QObject* _parent);
					virtual ~Entry();
					QSharedPointer<Block> getBlock( const QPoint& _Pos );
					bool isSameBlock( const QPoint& _posA, const QPoint& _posB ) const;
					QSize getDimension() const;
					QString getName() const;
					ID getBlockID( const QPoint& _pos ) const;
				private:
					QByteArray getMapData( ID _id );
					MapTiles decodeMap( QByteArray _data );
					StaticTiles decodeStatic( QByteArray _data );
					QString mName;
					QSize mDimension; // dimension counted in blocks!
					QDataStream mMapStream;
			};
			Facets( QObject* _parent );
			virtual ~Facets();
			Facets& addFacet( QString _name, QString _mapFile, QSize _dimension, QString _staticsIndex, QString _staticsData );
			QSharedPointer<Facets::Entry> getFacet( QString _name );
		private:
			QMap<QString, QSharedPointer<Entry> > mFacets;
	};

	typedef QSharedPointer<Facets::Entry> Facet;

	inline Facets::Block::Block( QPointer<Facets::Entry> _facet, QPoint _offset, MapTiles _mapTiles, StaticTiles _staticTiles )
	: QPair<MapTiles,StaticTiles>(_mapTiles,_staticTiles),
	  Object( Object::Facet, (_facet->getName().toAscii())+QByteArray::number(_facet->getBlockID(_offset)) ),
	  mFacet(_facet),
	  mOffset(_offset) {
	}

	inline Facets::Block::~Block() {
	}

	inline QPoint Facets::Block::getOffset() const {
		return mOffset;
	}

	inline QPointer<Facets::Entry> Facets::Block::getFacet() const {
		return mFacet;
	}

	inline bool Facets::Entry::isSameBlock( const QPoint& _posA, const QPoint& _posB ) const {
		return (getBlockID(_posA)==getBlockID(_posB)) ? true : false;
	}

	inline ID Facets::Entry::getBlockID( const QPoint& _pos ) const {
		return (_pos.x()/8*mDimension.height()) + _pos.y()/8;
	}

	inline QSize Facets::Entry::getDimension() const {
		return mDimension;
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
