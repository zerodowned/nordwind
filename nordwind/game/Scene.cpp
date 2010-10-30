#include "Scene.hpp"
#include "../data/IndexFileHandle.hpp"
#include <qapplication.h>
#include <qdatetime.h>
#include <qdebug.h>

using namespace game;

const QMatrix4x4 Scene::sMatrix( 22.f, 22.f, 4.f, 0.f,
								   -22.f, 22.f, 4.f, 0.f,
								     0.f, -4.f, 1.f, 0.f,
								     0.f,  0.f, 0.f, 1.f);

Scene::Scene(const QString& name,
		const QString& mapFile,
		const QString& staticsFile,
		const QString& indexFile,
		const QSize& dimension,
		const QSize& blockSize) :
	QGraphicsScene(),
	mMapFile(mapFile),
	mStaticsFile(staticsFile),
	mDimension(dimension),
	mBlockSize(blockSize) {
		setObjectName(name);
		setParent(qApp);
		if(!(mMapFile.open(QIODevice::ReadOnly)&&data::IndexFileHandle::instance(mStaticsFile,indexFile)))
			qWarning() << "Couldn't open map" << mapFile << "statics" << mStaticsFile << "index" << indexFile;
		else
			qDebug() << "Initialized map" << mapFile << "statics" << mStaticsFile << "index" << indexFile;
		mBlocks.setMaxCost(512);
}

Scene* Scene::instance(const QString& name) {
	return qApp->findChild<Scene*>(name);
}

Scene::~Scene(){
}

/** Reads from map file a raw map block
 *
 */
Scene::Block::Maps Scene::maps(ID blockID) {
	QDataStream stream(&mMapFile);
	stream.setByteOrder(QDataStream::LittleEndian);
	int tilesNum = mBlockSize.width() * mBlockSize.height();
	// seek to block number times block size (4 byte header + tileNum * sizeof(Map::Raw)) + 4 byte header
	if (!mMapFile.seek(blockID * (4 + tilesNum * 3) + 4))
		return Scene::Block::Maps();
	Scene::Block::Maps tmp(tilesNum);
	for (Scene::Block::Maps::iterator iter = tmp.begin(); iter != tmp.end(); iter++)
		stream >> iter->mID >> iter->mZ;
	return tmp;
}


/** Decodes a statics block from the statics file
 *
 */
Scene::Block::Statics Scene::statics(ID blockID) {
	Scene::Block::Statics result;
	QFile file(mStaticsFile + ":" + QString::number(blockID));
	if (file.open(QIODevice::ReadOnly)) {
		QDataStream stream(file.readAll());
		stream.setByteOrder(QDataStream::LittleEndian);;
		result.resize(file.size() / 7);
		for (Scene::Block::Statics::iterator iter = result.begin(); iter != result.end(); iter++)
			stream >> iter->mID >> iter->mXOffset >> iter->mYOffset >> iter->mZ
					>> iter->mHueID;
	} else
		qDebug() << "Statics block empty or out of boundary" << mStaticsFile << "index" << blockID;
	return result;
}

void Scene::loadMap(const QRect& rect) {
	QPoint topLeft = convertCellToBlockPoint(
			rect.topLeft());
	QPoint bottomRight = convertCellToBlockPoint(
			rect.bottomRight());
	for (QPoint p = topLeft; p.x() <= bottomRight.x(); p.rx()++)
		for (p.ry() = topLeft.y(); p.y() <= bottomRight.y(); p.ry()++)
			if (!mBlocks.contains(id(p)))
				constructBlock(p);
}

/** Set z values of east, south and down map tile neighbour
 *
 */
void Scene::setZValues(Scene::Block::Maps& maps,
		const Scene::Block::Maps& easts,
		const Scene::Block::Maps& downs,
		const Scene::Block::Maps& souths) {
	Scene::Block::Maps::const_iterator end = maps.end();
	Scene::Block::Maps::const_iterator east = easts.isEmpty()?maps.begin()+mBlockSize.width()-1:easts.begin();
	Scene::Block::Maps::const_iterator south = souths.isEmpty()?maps.end()-mBlockSize.width():souths.begin();
	Scene::Block::Maps::const_iterator down = downs.isEmpty()?maps.end()-1:downs.begin();
	int x = 1;
	for(Scene::Block::Maps::iterator iter = maps.begin(); iter!=end; iter++,x++) {
		if(x%mBlockSize.width()==0) {
			iter->mEastZ = east->mZ;
			east+=mBlockSize.width();
		} else
			iter->mEastZ = (iter+1)->mZ;
		iter->mSouthZ = (iter+mBlockSize.width()<end?(iter+mBlockSize.width()):south++)->mZ;
		iter->mDownZ = (iter+mBlockSize.width()+1<end?(iter+mBlockSize.width()+1):down++)->mZ;
	}
}

void Scene::constructBlock(const QPoint& pos) {
	QTime time;
	time.start();
	Block* block = new Block;
	ID ident = id(pos);
	QPoint p(pos.x()*mBlockSize.width(),pos.y()*mBlockSize.height());
	Scene::Block::Maps map = maps(ident);
	if(!map.isEmpty()) {
		setZValues(map,maps(id(pos+QPoint(1,0))),maps(id(pos+QPoint(1,1))),maps(id(pos+QPoint(0,1))));
		int i = 0;
		for(Scene::Block::Maps::iterator iter = map.begin(); iter < map.end(); i++, iter++) {
			QSharedPointer<QGraphicsItem> map(new Map(iter->mID,iter->mZ,iter->mSouthZ,iter->mEastZ,iter->mDownZ));
			QVector3D offset(p.x()+i%mBlockSize.width(),p.y()+i/mBlockSize.width(),iter->mZ);
			addItem(map.data(),offset);
			block->insert(offset, map);
		}
	}
	Scene::Block::Statics stats = statics(ident);
	for(Scene::Block::Statics::iterator iter =
			stats.begin(); iter != stats.end(); iter++) {
		QSharedPointer<QGraphicsItem> item(new Static(iter->mID,iter->mZ));
		QVector3D offset(p.x()+iter->mXOffset,p.y()+iter->mYOffset,iter->mZ);
		addItem(item.data(),offset);
		block->insert(offset, item);
	}
	mBlocks.insert(ident,block);
}
