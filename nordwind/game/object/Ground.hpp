/*
 * MapEntity.hpp
 *
 *  Created on: 16.12.2009
 *      Author: idstein
 */
#ifndef MAPENTITY_HPP_
#define MAPENTITY_HPP_

#include "../Object.hpp"
#include <QGraphicsItem>
#include <qgl.h>

namespace game {
    class Ground : public Object, public QGraphicsItem {
    public:
        Ground(const ID& id = 0x2);
        virtual ~Ground();
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        virtual QRectF boundingRect() const;
        virtual void setID(const ID &id);
        virtual int type() const;
        void stretch(Z southZ, Z eastZ, Z downZ);

        void setPos(const QPointF &pos);
        void setZValue(qreal z);
    protected:
        virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    private:
        bool mTex;
        GLfloat* mPolygon;
        QPixmap mTexture;
        static const GLfloat defaultVertexArray[4*2];
        static const GLfloat artsCoords[4*2];
        static const GLfloat textureCoords[4*2];
    };

    inline void Ground::setPos(const QPointF &pos) {
        return QGraphicsItem::setPos(pos);
    }

    inline void Ground::setZValue(qreal z) {
        return QGraphicsItem::setZValue(z);
    }
}

#endif /* MAPENTITY_HPP_ */
