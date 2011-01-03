/*
 * Mobile.hpp
 *
 *  Created on: 22.12.2009
 *      Author: idstein
 */

#ifndef MOBILE_HPP_
#define MOBILE_HPP_

#include "../Object.hpp"
#include "../Dynamic.hpp"
#include <QGraphicsPixmapItem>
#include <QMovie>

namespace game {

    class Mobile : public Dynamic,
    public Object,
    public QGraphicsPixmapItem {
        Q_OBJECT;
        Q_ENUMS(Direction);
        Q_ENUMS(Gender);
    public:
        enum Direction {
            West = 0,
            Right = 1,
            East = 2,
            Down = 3,
            South = 4,
            Left = 5,
            North = 6,
            Up = 7,
        } Q_PACKED;
        enum Gender {
            Male = 0x00,
            Female = 0x01
                 } Q_PACKED;
        static const quint8 sDirectionMap[8];
        static const bool sDirectionFlip[8];
        typedef quint8 Action;
        Mobile( const Serial& serial, QObject* parent, const ID& body = 0x190, const Direction& direction = Down, Action action = 0x4 );
        QRectF boundingRect() const;
        ID body() const;
        Action action() const;
        Direction direction() const;
        ID id() const;
        //void paint( QPainter* _painter, const QStyleOptionGraphicsItem* _option, QWidget* _widget = 0 );
        void setPos(const QPointF& pos);
        void setZValue(qreal z);
    public Q_SLOTS:
        void onFrameChanged( int frameNumber );
    protected:
        Action mAction;
        Direction mDirection;
        QMovie mBase;
        QString mName;
        quint16 mCurrentHP, mMaximumHP,
        mCurrentStamina, mMaximumStamina,
        mCurrentMana, mMaximumMana;
        Gender mGender;
        quint16 mStrength, mDexterity, mIntelligence;
        quint16 mGold, mWeight, mStatCap;
        quint16 mResistPhysical, mResistFire, mResistCold, mResistPoison, mResistEnergy;
        quint16 mLuck;
        quint16 mMinDamage, mMaxDamage;
        quint16 mTithingPoints;
    };

    inline void Mobile::setPos(const QPointF &pos) {
        QGraphicsItem::setPos(pos);
    }

    inline void Mobile::setZValue(qreal z) {
        QGraphicsItem::setZValue(z);
    }

}

#endif /* MOBILE_HPP_ */
