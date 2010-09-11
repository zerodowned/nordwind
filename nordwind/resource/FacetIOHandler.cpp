#include "FacetIOHandler.hpp"
#include "IndexFileHandle.hpp"
#include "../Client.hpp"
#include <qdebug.h>

using namespace resource;

QHash<QString, QSharedPointer<FacetIOHandler> > FacetIOHandler::sFacets;

QSharedPointer<FacetIOHandler> resource::FacetIOHandler::instance(
		const QString & name, QObject *parent) {
	if (!sFacets.contains(name)) {
		QSettings& settings(Client::getInstance()->settings());
		settings.beginGroup("facets/" + name);
		QString mapFile = settings.value("map").toString();
		QString staticsFile = settings.value("data").toString();
		QString indexFile = settings.value("index").toString();
		QSize size = settings.value("size").toSize();
		FacetIOHandler* facet = new FacetIOHandler(name, mapFile, staticsFile,
				indexFile, size, parent);
		if (!facet->open(QIODevice::ReadOnly)) {
			qWarning() << "Unable to load facet " << name;
			delete facet;
		} else
			qDebug() << "Facet" << name << "loaded.";
		settings.endGroup();
	}
	return sFacets[name];
}

FacetIOHandler::FacetIOHandler(const QString & name, const QString & mapFile,
		const QString & staticsFile, const QString & indexFile,
		const QSize & dimension, QObject *parent) :
	QFile(mapFile, parent), mName(name), mDimension(dimension), mStaticsFile(
			staticsFile), mIndexFile(indexFile) {
	sFacets.insert(name, QSharedPointer<FacetIOHandler> (this));
}

FacetIOHandler::~FacetIOHandler() {
}

bool FacetIOHandler::open(QIODevice::OpenMode mode) {
	if (!IndexFileHandle::instance(mStaticsFile, mIndexFile))
		return false;
	return QFile::open(mode);
}

QSize FacetIOHandler::sBlockSize = QSize(8, 8);

FacetIOHandler::Maps FacetIOHandler::operator [](ID blockID) {
	QDataStream stream(this);
	stream.setByteOrder(QDataStream::LittleEndian);
	int tilesNum = sBlockSize.width() * sBlockSize.height();
	if (!seek(blockID * (4 + tilesNum * 3) + 4))
		return FacetIOHandler::Maps();
	QVector<FacetIOHandler::Map> tmp(tilesNum);
	for (QVector<FacetIOHandler::Map>::iterator iter = tmp.begin(); iter != tmp.end(); iter++)
		stream >> iter->mID >> iter->mZ;
	return FacetIOHandler::Maps::fromVector(tmp);
}

FacetIOHandler::Statics FacetIOHandler::statics(ID blockID) {
	FacetIOHandler::Statics result;
	QFile file(mStaticsFile + ":" + QString::number(blockID));
	if (file.open(QIODevice::ReadOnly)) {
		QDataStream stream(file.readAll());
		stream.setByteOrder(QDataStream::LittleEndian);;
		result.resize(file.size() / 7);
		for (Statics::iterator iter = result.begin(); iter != result.end(); iter++)
			stream >> iter->mID >> iter->mXOffset >> iter->mYOffset >> iter->mZ
					>> iter->mHueID;
	} else
		qDebug() << "Unable to open" << mStaticsFile << "index" << blockID;
	return result;
}

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
FacetIOHandler::Block* FacetIOHandler::block(QPoint point) {
	ID i = id(point);
	FacetIOHandler::Block* block = new FacetIOHandler::Block;
	block->mBlockOffset = QPoint(point.x() * sBlockSize.width(), point.y()
			* sBlockSize.height());
	block->mMap = operator[](i);
	if(!block->mMap.isEmpty()) {
		// necessary for strech calculation and thereby avoid additional recursive loads
		// QList instead of QVector due to performance concerning insert/append operations
		FacetIOHandler::Maps east = operator[](id(point + QPoint(1, 0)));
		for (int y = 0; y < sBlockSize.height(); y++)
			block->mMap.insert(FacetIOHandler::Block::index(QPoint(sBlockSize.width(),y)),
			east.isEmpty()?block->mMap[FacetIOHandler::Block::index(QPoint(sBlockSize.width()-1,y))]:east[y*sBlockSize.width()]);
		FacetIOHandler::Maps south = operator[](id(point + QPoint(0, 1)));
		block->mMap += south.isEmpty() ? block->mMap.mid((sBlockSize.height()
				- 1) * sBlockSize.width(), sBlockSize.width()) : south.mid(0,sBlockSize.width());
		FacetIOHandler::Maps down = operator[](id(point + QPoint(1, 1)));
		block->mMap += down.isEmpty() ? block->mMap.back() : down.front();
	}
	block->mStatics = statics(i);
	return block;
}

//Facets::Block Facets::Entry::getBlock(const QPoint& _blockPos) {
//	ID id = convertBlockPosToID(_blockPos);
//
//	if (!mCache.contains(id)) {
//		MapTiles mapTiles = decodeMap(getMapData(id));
//		if (!mapTiles.isEmpty()) {
//			MapTiles southTiles = decodeMap(getMapData(convertBlockPosToID(
//					_blockPos + QPoint(0, 1))));
//			for (qint32 x = 0; x < mBlockSize.width(); x++) {
//				mapTiles.insert(QPoint(x, mBlockSize.height()),
//						!southTiles.isEmpty() ? southTiles[QPoint(x, 0)]
//								: mapTiles[QPoint(x, mBlockSize.height() - 1)]);
//			}
//
//			MapTiles eastTiles = decodeMap(getMapData(convertBlockPosToID(
//					_blockPos + QPoint(1, 0))));
//			for (qint32 y = 0; y < mBlockSize.height(); y++) {
//				mapTiles.insert(QPoint(mBlockSize.width(), y),
//						!eastTiles.empty() ? eastTiles[QPoint(0, y)]
//								: mapTiles[QPoint(mBlockSize.width() - 1, y)]);
//			}
//
//			MapTiles downTiles = decodeMap(getMapData(convertBlockPosToID(
//					_blockPos + QPoint(1, 1))));
//			mapTiles.insert(QPoint(mBlockSize.width(), mBlockSize.height()),
//					!downTiles.isEmpty() ? downTiles[QPoint(0, 0)]
//							: mapTiles[QPoint(mBlockSize.width() - 1,
//									mBlockSize.height() - 1)]);
//		}
//
//		Facets::Block block(QPoint(_blockPos.x() * mBlockSize.width(),
//				_blockPos.y() * mBlockSize.height()), mapTiles, decodeStatic(
//				getData(id)));
//		mCache.insert(id, new Block(block));
//		return block;
//	}
//	return Block(*(mCache[id]));
//}
