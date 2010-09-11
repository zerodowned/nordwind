#ifndef FACETIOHANDLER_HPP__
#define FACETIOHANDLER_HPP__

#include "../Typedefs.hpp"
#include <qfile.h>
#include <qsharedpointer.h>
#include <qhash.h>
#include <qcache.h>
#include <qvector.h>
#include <qsize.h>
#include <qpoint.h>

namespace resource {

class FacetIOHandler: public QFile {
public:
	static QSharedPointer<FacetIOHandler> instance(const QString& name,
			QObject* parent = NULL);
	explicit FacetIOHandler(const QString& name,
					const QString& mapFile, const QString& staticsFile,
					const QString& indexFile, const QSize& dimension, QObject* parent =
							NULL);
	virtual ~FacetIOHandler();
	struct Map {
		quint16 mID;
		qint8 mZ;
	};
	typedef QList<Map> Maps;
	Maps operator[](ID blockID);
	struct Static {
		quint16 mID;
		quint8 mXOffset;
		quint8 mYOffset;
		qint8 mZ;
		quint16 mHueID;
	};
	typedef QVector<Static> Statics;
	Statics statics(ID blockID);
	struct Block {
		QPoint mBlockOffset;
		Maps mMap;
		static int index(const QPoint& p);
		Statics mStatics;
		QPoint relative(const QPoint& p) const;
	};
	Block* block(QPoint point);

	QSize dimension() const;
	static QSize blockSize();
	QString name() const;
	ID id(const QPoint& point) const;
	static QPoint convertCellToBlockPoint(const QPoint& point);
	bool open(QIODevice::OpenMode mode);
	static QSize sBlockSize;
private:
	QString mName;
	QSize mDimension; // dimension counted in blocks!
	QString mStaticsFile;
	QString mIndexFile;
	static QHash<QString, QSharedPointer<FacetIOHandler> > sFacets;
};

inline int FacetIOHandler::Block::index(const QPoint& p) {
	return p.y()*FacetIOHandler::sBlockSize.width()+p.y()+p.x();
}

inline QPoint FacetIOHandler::Block::relative(const QPoint& p) const {
	return mBlockOffset + p;
}

inline QPoint FacetIOHandler::convertCellToBlockPoint(const QPoint& point) {
	return QPoint(point.x() / sBlockSize.width(), point.y()
			/ sBlockSize.height());
}

inline ID FacetIOHandler::id(const QPoint& point) const {
	return (point.x() * mDimension.height()) + point.y();
}

inline QSize FacetIOHandler::dimension() const {
	return mDimension;
}

inline QSize FacetIOHandler::blockSize() {
	return sBlockSize;
}

inline QString FacetIOHandler::name() const {
	return mName;
}

}
#endif // FACETIOHANDLER_HPP__
