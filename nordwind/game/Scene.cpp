#include "Scene.hpp"
#include "Map.hpp"
#include "Static.hpp"
#include "../resource/FacetIOHandler.hpp"
#include <qdatetime.h>
#include <qdebug.h>

using namespace game;

Scene::Scene(const QString& name, QObject* _parent) :
	QGraphicsScene(_parent), mFacet(resource::FacetIOHandler::instance(name)) {
		mBlocks.setMaxCost(512);
}

void Scene::loadMap(const QRect& rect) {
	if (!mFacet) {
		qWarning() << "No facet loaded";
		return;
	}
	QPoint topLeft = resource::FacetIOHandler::convertCellToBlockPoint(
			rect.topLeft());
	QPoint bottomRight = resource::FacetIOHandler::convertCellToBlockPoint(
			rect.bottomRight());
	for (QPoint p = topLeft; p.x() <= bottomRight.x(); p.rx()++)
		for (p.ry() = topLeft.y(); p.y() <= bottomRight.y(); p.ry()++)
			if (!mBlocks.contains(mFacet->id(p)))
				constructBlock(p);
}

void Scene::constructBlock(QPoint point) {
	QTime time;
	time.start();
	Block* block = new Block;
	QScopedPointer<resource::FacetIOHandler::Block> raw(mFacet->block(point));
	resource::FacetIOHandler::Maps mapTiles = raw->mMap;
	if(!raw->mMap.isEmpty())
		for(quint8 y = 0; y < resource::FacetIOHandler::sBlockSize.height(); y++)
			for(quint8 x = 0; x < resource::FacetIOHandler::sBlockSize.width(); x++) {
				QPoint p(x,y);
				const resource::FacetIOHandler::Map& rawMap = mapTiles[raw->index(p)];
				QSharedPointer<Entity> map(new Map(
						raw->relative(p),
						rawMap.mID,
						rawMap.mZ,
						mapTiles[raw->index(p+QPoint(0,1))].mZ, // South
						mapTiles[raw->index(p+QPoint(1,1))].mZ, // Down
						mapTiles[raw->index(p+QPoint(1,0))].mZ)); // East
				addItem(map.data());
				block->insert(p, map);
			}
	for (resource::FacetIOHandler::Statics::iterator iter =
			raw->mStatics.begin(); iter != raw->mStatics.end(); iter++) {
		QPoint p(iter->mXOffset, iter->mYOffset);
		QSharedPointer<Entity> item(new Static(Coordinate(
				raw->relative(p), iter->mZ), iter->mID, iter->mHueID));
		addItem(item.data());
		block->insert(p, item);
	}
	mBlocks.insert(mFacet->id(point),block);
}

void Scene::connectNotify(const char* _signal) {
	if (QLatin1String(_signal)==SLOT(changed(const QList<QRectF>&))) {

	}
}
