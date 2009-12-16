#ifndef CMAPS_H_
#define CMAPS_H_

#include "IndexFile.hpp"
#include "Object.hpp"
#include <qcache.h>
#include <qmap.h>

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
			typedef QPair<MapTiles,StaticTiles> Block;
			class Entry : public IndexFile {
				public:
					Entry( QString _name, QString _mapFile, QSize _dimension, QString _indexFile, QString _dataFile, QObject* _parent);
					virtual ~Entry();
					QSharedPointer<Block> getBlock( const QPoint& _blockPos );
					bool isSameBlock( const QPoint& _posA, const QPoint& _posB ) const;
					QSize getDimension() const;
					QString getName() const;
				private:
					QByteArray getMapData( ID _id );
					MapTiles decodeMap( QByteArray _data );
					StaticTiles decodeStatic( QByteArray _data );
					ID getBlockID( const QPoint& _pos ) const;
					QString mName;
					QSize mDimension; // dimension counted in blocks!
					QDataStream mMapStream;
					QCache< ID, QSharedPointer<Block> > mBlockCache;
			};
			Facets( QObject* _parent );
			virtual ~Facets();
			Facets& addFacet( QString _name, QString _mapFile, QSize _dimension, QString _staticsIndex, QString _staticsData );
			QSharedPointer<Facets::Entry> getFacet( QString _name );
		private:
			QMap<QString, QSharedPointer<Entry> > mFacets;
	};

	typedef QSharedPointer<Facets::Entry> Facet;

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

	inline Facets& Facets::addFacet( QString _name, QString _mapFile, QSize _dimension, QString _staticsIndex, QString _staticsData ) {
		mFacets[_name] = Facet( new Facets::Entry(_name, _mapFile, _dimension, _staticsIndex, _staticsData, this ) );
		return *this;
	}

	inline Facet Facets::getFacet( QString _name ) {
		return mFacets.value(_name);
	}
}
#endif
