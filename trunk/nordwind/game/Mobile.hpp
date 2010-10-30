/*
 * Mobile.hpp
 *
 *  Created on: 22.12.2009
 *      Author: idstein
 */

#ifndef MOBILE_HPP_
#define MOBILE_HPP_

#include "Dynamic.hpp"
#include "../Typedefs.hpp"
#include <qmovie.h>

namespace game {
// TODO create a subclass for running animations here
class Mobile: public Dynamic {
Q_OBJECT;
public:
	enum Type {
		Monster,
		Animal,
		Character
	} Q_PACKED;
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
	typedef ID Body;
	Mobile( Body body = 0x190, Action action = 0x4, Direction direction = Down );
	QRectF boundingRect() const;
	Body body() const;
	Action action() const;
	Direction direction() const;
	ID id() const;
	void paint( QPainter* _painter, const QStyleOptionGraphicsItem* _option, QWidget* _widget = 0 );
public Q_SLOTS:
	void onFrameChanged( int frameNumber );
protected:
	Body mBody;
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

inline Mobile::Body Mobile::body() const {
	return mBody;
}

inline Mobile::Action Mobile::action() const {
	return mAction;
}

inline Mobile::Direction Mobile::direction() const {
	return mDirection;
}

inline QRectF Mobile::boundingRect() const {
	return mBase.frameRect();
}

}

#endif /* MOBILE_HPP_ */
