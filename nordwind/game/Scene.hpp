#ifndef SCENE_HPP__
#define SCENE_HPP__

#include <qgraphicsscene.h>
#include <qcache.h>
#include <qmap.h>
#include <qpoint.h>
#include <qfile.h>
#include <qvector3d.h>
#include <qmatrix4x4.h>
#include <qsharedpointer.h>
#include "Map.hpp"
#include "Static.hpp"
#include "../Typedefs.hpp"

// required for QMultiMap<QVector3D,...>
inline bool operator<(const QVector3D& v1, const QVector3D& v2) {
	return v1.x() < v2.x() || 
		  (v1.x() == v2.x() && v1.y() < v2.y()) ||
		  (v1.x()==v2.x()&&v1.y()==v2.y()&&v1.z()<v2.z());
}

namespace game {
class Scene;
class Scene: public QGraphicsScene {
Q_OBJECT;
public:
	class Block : public QMultiMap<QVector3D, QSharedPointer<QGraphicsItem> > {
	public:
		struct Map {
			quint16 mID;
			qint8 mZ;
			qint8 mSouthZ;
			qint8 mEastZ;
			qint8 mDownZ;
		};
		struct Static {
			quint16 mID;
			quint8 mXOffset;
			quint8 mYOffset;
			qint8 mZ;
			quint16 mHueID;
		};
		typedef QVector<Map> Maps;
		typedef QVector<Static> Statics;
	};
	explicit Scene(const QString& name, const QString& mapFile, const QString& staticsFile, const QString& indexFile, const QSize& dimension, const QSize& blockSize = QSize(8,8));
	virtual ~Scene();
	static Scene* instance(const QString& name);
	void loadMap( const QRect& rect );
	QSize dimension() const;
	QSize blockSize() const;
	void addItem(QGraphicsItem *item, const QVector3D& position);
protected:
	static QVector3D absolutePosition(const QPoint& p, Z z);
	void constructBlock(const QPoint& point);
	Block::Maps maps(ID blockID);
	void setZValues(Scene::Block::Maps& maps,
			const Scene::Block::Maps& easts,
			const Scene::Block::Maps& downs,
			const Scene::Block::Maps& souths);
	int mapOffset(int x, int y) const;
	Block::Statics statics(ID blockID);
	ID id(const QPoint& point) const;
	QPoint convertCellToBlockPoint(const QPoint& point) const;
private:
	QFile mMapFile;
	const QString mStaticsFile;
	const QSize mDimension;
	const QSize mBlockSize;
	QCache< ID, Block > mBlocks;
	static const QMatrix4x4 sMatrix;
};

inline QSize Scene::dimension() const {
	return mDimension;
}

inline QSize Scene::blockSize() const {
	return mBlockSize;
}

inline ID Scene::id(const QPoint& p) const {
	return (p.x() * mDimension.height()) + p.y();
}

inline QPoint Scene::convertCellToBlockPoint(const QPoint& p) const {
	return QPoint(p.x() / mBlockSize.width(), p.y()
			/ mBlockSize.height());
}

inline int Scene::mapOffset(int x, int y) const {
	return y*mBlockSize.width() + x;
}

inline void Scene::addItem(QGraphicsItem *item, const QVector3D& position) {
	QVector3D vec= position*sMatrix;
	QGraphicsScene::addItem(item);
	item->setPos(vec.x(),vec.y());
	item->setZValue(position.z());
}
}
#endif
