#ifndef SCENE_HPP__
#define SCENE_HPP__

#include "Object.hpp"
#include "object/Ground.hpp"

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QCache>
#include <QPoint>
#include <QDataStream>
#include <QVector3D>
#include <QMatrix4x4>
#include <QMultiHash>

inline uint qHash(const QPoint& p) {
    uint h1 = qHash(p.x());
    uint h2 = qHash(p.y());
    return ((h1 << 16) | (h1 >> 16)) ^ h2;
}

namespace game {

    class Scene: public QGraphicsScene {
        Q_OBJECT;
    public:
        explicit Scene(const QString& mapFile,
                       const QString& staticsFile,
                       const QString& indexFile,
                       QObject* parent = NULL,
                       const QSize& dimension = QSize(768,512));
        virtual ~Scene();
        QSize dimension() const;
        static QSize blockSize();
        static const QSize sBlockSize;
    public Q_SLOTS:
        void enableNetwork(QObject* network);
        void removeDynamic(quint32 serial);
        void loadMap(const QRect& rect);
        QGraphicsItem* updateItem(quint32 serial, quint16 artwork,const QVector3D& pos, quint16 hue, quint16 amount, quint8 direction, quint8 status);
        QGraphicsItem* updateMobile(quint32 serial, quint16 body, quint8 direction, const QVector3D& pos, quint16 hue, quint8 status, quint8 notoriety);
        QGraphicsItem* teleportMobile(quint32 serial, quint16 body, quint8 direction, const QVector3D& pos, quint16 hue, quint8 status );
        void showUnicodeText(quint32 serial, quint16 body, quint8 mode, quint16 hue, quint16 font, QByteArray lang, QString name, QString message);
    protected:
        QList<QGraphicsItem*> maps(const ID& blockID, const QPoint& blockCoord);
        QList<QGraphicsItem*> statics(const ID& blockID, const QPoint& blockCoord);
        quint32 id(const QPoint& point) const;
    private:
        const QString mStaticsFile;
        const QSize mDimension;
        QDataStream mMapStream;

        QCache<ID, QRect> mBlocks;
        QCache<Serial, QObject> mDynamics;
        QMultiHash<QPoint,QGraphicsItem*> mIndex;
        QHash<QPoint,Ground*> mMap;
    };

}
#endif
