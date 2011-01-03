#include "Scene.hpp"
#include <QDir>
#include <QDebug>

#include <QGraphicsSimpleTextItem>
#include "object/Ground.hpp"
#include "object/Static.hpp"
#include "object/Item.hpp"
#include "object/Mobile.hpp"

using namespace game;

const QSize Scene::sBlockSize = QSize(8,8);

Scene::Scene(const QString& mapFile,
             const QString& staticsFile,
             const QString& indexFile,
             QObject* parent,
             const QSize& dimension) :
QGraphicsScene(parent),
mStaticsFile(staticsFile),
mDimension(dimension) {
    mMapStream.setDevice(new QFile(mapFile,this));
    mMapStream.setByteOrder(QDataStream::LittleEndian);
    if(!mMapStream.device()->open(QIODevice::ReadOnly))
        qWarning() << mMapStream.device()->errorString();
    else if(!QFile::exists(staticsFile))
        qWarning() << staticsFile << "does not exist!";
    else
        qDebug() << "Initialized map" << mapFile << "statics" << mStaticsFile << "index" << indexFile;
    setItemIndexMethod(QGraphicsScene::NoIndex);
    setSortCacheEnabled(false);
    //mBlocks.setMaxCost(512);
    //setProperty("map.load",true);
    //setProperty("static.load",false);
}

Scene::~Scene(){
}

QSize Scene::dimension() const {
    return mDimension;
}

QSize Scene::blockSize() {
    return sBlockSize;
}

ID Scene::id(const QPoint& p) const {
    if(p.x()<0 || p.x()>=mDimension.width() || p.y() < 0 || p.y()>=mDimension.height())
        return -1;
    else
        return (p.x() * mDimension.height()) + p.y();
}

struct RawMap {
    quint16 mID;
    qint8 mZ;
};

QVector<RawMap> helper_mapLoad(QDataStream& stream, const ID& blockID, const int& num = 64) {
    QVector<RawMap> result;
    if(blockID!=-1&&
       // seek to block number times block size (4 byte header + tileNum * sizeof(Map::Raw)) + 4 byte header
       stream.device()->seek(blockID * (4 + num * 3) + 4)) {
        result.resize(num);
        for (QVector<RawMap>::iterator iter = result.begin(); iter != result.end(); iter++)
            stream >> iter->mID >> iter->mZ;
    }
    return result;
}

/** Reads from map file a raw map block
 *
 */
QList<QGraphicsItem*> Scene::maps(const ID& id, const QPoint& blockCoord) {
    QList<QGraphicsItem*> items;

    QVector<RawMap> maps = helper_mapLoad(mMapStream,id);
    if(maps.isEmpty())
        return items;

    QVector<RawMap> souths = helper_mapLoad(mMapStream,Scene::id(blockCoord+QPoint(0,1)));
    QVector<RawMap> easts = helper_mapLoad(mMapStream,Scene::id(blockCoord+QPoint(1,0)));
    QVector<RawMap> downs = helper_mapLoad(mMapStream,Scene::id(blockCoord+QPoint(1,1)));

    QVector<RawMap>::const_iterator cur = maps.constBegin();
    QVector<RawMap>::const_iterator curend = maps.constEnd();
    QVector<RawMap>::const_iterator south = souths.isEmpty()?maps.constEnd()-sBlockSize.width():souths.constBegin();
    QVector<RawMap>::const_iterator southend = souths.isEmpty()?maps.constEnd():souths.constEnd();
    QVector<RawMap>::const_iterator east = easts.isEmpty()?maps.constBegin()+sBlockSize.width()-1:easts.constBegin();
    QVector<RawMap>::const_iterator eastend = easts.isEmpty()?maps.constEnd():easts.constEnd();
    QVector<RawMap>::const_iterator down = downs.isEmpty()?maps.constEnd()-1:downs.constBegin();

    QPoint blockOffset(blockCoord.x()*sBlockSize.width(),blockCoord.y()*sBlockSize.height());
    Z southZ,eastZ,downZ;
    for(int i = 0; cur != curend; i++,cur++) {
        QPoint offset(i%sBlockSize.width(),i/sBlockSize.width());
        eastZ = qMin(cur+1,curend-1)->mZ;
        southZ = qMin(cur+sBlockSize.width(),curend-1)->mZ;
        downZ = qMin(cur+sBlockSize.width()+1,curend-1)->mZ;

        game::Ground* item = new game::Ground(cur->mID);
        if(offset.y()==(sBlockSize.height()-1)) {
            southZ = south->mZ;
            downZ = qMin(south+1,southend)->mZ;
            south++;
        }
        if(offset.x()==(sBlockSize.width()-1)) {
            eastZ = east->mZ;
            downZ = qMin(east+sBlockSize.width(),eastend)->mZ;
            east += sBlockSize.width();
        }
        if(cur==curend-1)
            downZ = down->mZ;
        item->setCoordinates(QVector3D(blockOffset.x()+i%sBlockSize.width(),
                                       blockOffset.y()+i/sBlockSize.width(),
                                       cur->mZ));
        item->stretch(southZ,eastZ,downZ);
        items.append(item);
    }
    return items;
}

/** Decodes a statics block from the statics file
 *
 */
QList<QGraphicsItem*> Scene::statics(const ID& blockID, const QPoint& blockCoord) {
    QList<QGraphicsItem*> items;
    if(blockID==-1)
        return items;

    QFile file(mStaticsFile + QDir::separator() + QString::number(blockID));
    if(file.open(QIODevice::ReadOnly)) {
        QDataStream stream(file.readAll());
        stream.setByteOrder(QDataStream::LittleEndian);
        struct RawStatic {
            quint16 mID;
            quint8 mXOffset;
            quint8 mYOffset;
            qint8 mZ;
            quint16 mHueID;
        } temp;
        QPoint blockOffset(blockCoord.x()*sBlockSize.width(),blockCoord.y()*sBlockSize.height());
        for (int count = file.size()/7; count != 0; count--) {
            stream >> temp.mID >> temp.mXOffset >> temp.mYOffset >> temp.mZ >> temp.mHueID;
            Static* item = new game::Static(temp.mID,temp.mHueID);
            item->setCoordinates(QVector3D(blockOffset.x()+temp.mXOffset,
                                           blockOffset.y()+temp.mYOffset,
                                           temp.mZ));
            items.push_back(item);
        }
    }
    return items;
}

QPoint helper_convertCellToBlockPoint(const QPoint& p) {
    return QPoint(p.x() / Scene::sBlockSize.width(), p.y()
                  / Scene::sBlockSize.height());
}

void Scene::loadMap(const QRect& rect) {
    QPoint topLeft = helper_convertCellToBlockPoint(rect.topLeft());
    QPoint bottomRight = helper_convertCellToBlockPoint(rect.bottomRight());
    QList<QGraphicsItem*> objects;
    for (QPoint p = topLeft; p.x() <= bottomRight.x(); p.rx()++) {
        for (p.ry() = topLeft.y(); p.y() <= bottomRight.y(); p.ry()++) {
            ID blockID = id(p);
            if (!mBlocks.contains(blockID)) {
                if(!property("map.discard").toBool())
                    objects.append(maps(blockID,p));
                if(!property("static.discard").toBool())
                    objects.append(statics(blockID,p));
            }
        }
    }
    // new items
    Q_FOREACH(QGraphicsItem* item, objects) {
        addItem(item);
    }
}

void Scene::enableNetwork(QObject *network) {
    connect(network,SIGNAL(mobileTeleported(quint32,quint16,quint8,QVector3D,quint16,quint8)),
            SLOT(teleportMobile(quint32,quint16,quint8,QVector3D,quint16,quint8)));
    connect(network,SIGNAL(itemUpdated(quint32,quint16,QVector3D,quint16,quint16,quint8,quint8)),
            SLOT(updateItem(quint32,quint16,QVector3D,quint16,quint16,quint8,quint8)));
    connect(network,SIGNAL(dynamicRemoved(quint32)),
            SLOT(removeDynamic(quint32)));
    connect(network,SIGNAL(mobileUpdated(quint32,quint16,quint8,QVector3D,quint16,quint8,quint8)),
            SLOT(updateMobile(quint32,quint16,quint8,QVector3D,quint16,quint8,quint8)));
    connect(network,SIGNAL(unicodeTextReceived(quint32,quint16,quint8,quint16,quint16,QByteArray,QString,QString)),
            SLOT(showUnicodeText(quint32,quint16,quint8,quint16,quint16,QByteArray,QString,QString)));
}

QGraphicsItem* Scene::updateItem(quint32 serial, quint16 art, const QVector3D &pos, quint16 hue, quint16 amount, quint8 direction, quint8 status) {
    Item* item = qobject_cast<Item*>(mDynamics.take(serial));
    if(!item) {
        item = new Item(serial,sender(),art,hue);
        // TODO add hue and id data
        item->setCoordinates(pos);
        addItem(item);
        mDynamics.insert(serial,item);
    } else {
        Item* obj = qgraphicsitem_cast<Item*>(item);
        obj->setCoordinates(pos);
    }
    return item;
}

void Scene::removeDynamic(quint32 serial) {
    mDynamics.remove(serial);
}

void Scene::showUnicodeText(quint32 serial, quint16 body, quint8 mode, quint16 hue, quint16 font, QByteArray lang, QString name, QString message) {
    Mobile* mobile = qobject_cast<Mobile*>(mDynamics.take(serial));
    if(mobile) {
        QGraphicsSimpleTextItem* text = new QGraphicsSimpleTextItem(message);
        text->setPos(mobile->pos()+QPointF(0.0f,-50.0f));
        text->setZValue(mobile->zValue()+200.0f);
    }
    qDebug() << Q_FUNC_INFO;
    qDebug() << name << message;
}

QGraphicsItem* Scene::teleportMobile(quint32 serial, quint16 body, quint8 direction, const QVector3D &pos, quint16 hue, quint8 status) {
    Mobile* item = qobject_cast<Mobile*>(mDynamics.take(serial));
    if(item)
        item->setCoordinates(pos);
    return item;
}

QGraphicsItem* Scene::updateMobile(quint32 serial, quint16 body, quint8 direction, const QVector3D &pos, quint16 hue, quint8 status, quint8 notoriety) {
    Mobile* mobile = qobject_cast<Mobile*>(mDynamics.take(serial));
    if(!mobile) {
        mobile = new Mobile(serial,sender(),body,(Mobile::Direction)direction,status);
        mobile->setCoordinates(pos);
        addItem(mobile);
    } else {
     //   placeItemAt(mobile);
    }
    return mobile;
}
