/*
 * Mobile.hpp
 *
 *  Created on: 22.12.2009
 *      Author: idstein
 */

#ifndef MOBILE_HPP_
#define MOBILE_HPP_

#include "Dynamic.hpp"
#include "../core/Resources.hpp"
#include <qmovie.h>

namespace game {
// TODO create a subclass for running animations here
class Mobile: public Dynamic {
Q_OBJECT	;
public:
	enum Type {
		Monster,
		Animal,
		Character
	};
	enum Direction {
		West = 0,
		Right = 1,
		East = 2,
		Down = 3,
		South = 4,
		Left = 5,
		North = 6,
		Up = 7,
	};
	typedef quint8 Action;
	typedef ID Body;
	Mobile( const QPoint& point, Z _z, Body body, Hue hue, Action action = 0x4, Direction direction = Down );
	Body body() const;
	Action action() const;
	Direction direction() const;
	Hue hue() const;
	ID id() const;
	//void paint( QPainter* _painter, const QStyleOptionGraphicsItem* _option, QWidget* _widget = 0 );
public Q_SLOTS:
	void onFrameChanged( int frameNumber );
private:
	Body mBody;
	Action mAction;
	Direction mDirection;
	Hue mHue;
	QMovie mBase;
	static const quint8 sDirectionMap[8];
	static const bool sDirectionFlip[8];
	static QMap<Type,QPair<ID,quint8> > sTypeMap;
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

inline Hue Mobile::hue() const {
	return mHue;
}

}

#endif /* MOBILE_HPP_ */
