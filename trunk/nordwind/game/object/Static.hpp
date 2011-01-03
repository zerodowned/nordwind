/*
 * StaticEntity.hpp
 *
 *  Created on: 18.12.2009
 *      Author: idstein
 */

#ifndef STATICENTITY_HPP_
#define STATICENTITY_HPP_

#include "../Object.hpp"
#include <QGraphicsPixmapItem>

namespace game {
    class Static : public Object, public QGraphicsPixmapItem {
    public:
        Static(const ID& id = 0,const ID& hue = 0);
        virtual void setID(const ID &id);
        ID hue() const;
        virtual void setHue(const ID& hue);

        void setPos(const QPointF &pos);
        void setZValue(qreal z);
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    protected:
        void updatePixmap(const ID& newID = 0, const ID& newHue = 0);
        QVariant itemChange ( QGraphicsItem::GraphicsItemChange change, const QVariant & value );
    private:
        ID mHue;
    };

    inline void Static::setPos(const QPointF &pos) {
        return QGraphicsPixmapItem::setPos(pos);
    }

    inline void Static::setZValue(qreal z) {
        return QGraphicsPixmapItem::setZValue(z);
    }
}

#endif /* STATICENTITY_HPP_ */
