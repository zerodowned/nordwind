/*
 * Coordinate.hpp
 *
 *  Created on: 23.12.2009
 *      Author: idstein
 */

#ifndef COORDINATE_HPP_
#define COORDINATE_HPP_

#include <qglobal.h>

namespace game {
	class Coordinate {
		public:
			Coordinate();
			Coordinate(quint32 _x, quint32 _y, qint32 _z);
			Coordinate(const QPoint& _p, qint32 _z);
			Coordinate(const QPoint& _p);
			Coordinate(const Coordinate& _o);
			quint32 x() const;
			Coordinate& setX(quint32 _x);
			quint32 y() const;
			Coordinate& setY(quint32 _y);
			qint32 z() const;
			Coordinate& setZ(qint32 _z);
			Coordinate& operator=(const Coordinate& _o);
			Coordinate& operator=(const QPoint& _p);
			Coordinate& operator+=(const Coordinate& _s);
			Coordinate& operator+=(const QPoint& _o);
			bool operator<(const Coordinate& _o) const;
			operator QString() const;
			QPointF toScenePoint() const;
			qreal toSceneZValue() const;
		private:
			quint32 mX;
			quint32 mY;
			qint32 mZ;
	};

	inline Coordinate::Coordinate()
	: mX(0),
	  mY(0),
	  mZ(0) {
	}

	inline Coordinate::Coordinate( quint32 _x, quint32 _y, qint32 _z )
	: mX(_x),
	  mY(_y),
	  mZ(_z) {
	}

	inline Coordinate::Coordinate( const QPoint& _p, qint32 _z )
	: mX(_p.x()),
	  mY(_p.y()),
	  mZ(_z) {
	}

	inline Coordinate::Coordinate( const QPoint& _p )
	: mX(_p.x()),
	  mY(_p.y()),
	  mZ(0) {
	}

	inline Coordinate::Coordinate( const Coordinate& _other )
	: mX(_other.mX),
	  mY(_other.mY),
	  mZ(_other.mZ) {
	}

	inline quint32 Coordinate::x() const {
		return mX;
	}

	inline Coordinate& Coordinate::setX( quint32 _x ) {
		mX = _x;
		return *this;
	}

	inline quint32 Coordinate::y() const {
		return mY;
	}

	inline Coordinate& Coordinate::setY( quint32 _y ) {
		mY = _y;
		return *this;
	}

	inline qint32 Coordinate::z() const {
		return mZ;
	}

	inline Coordinate& Coordinate::setZ( qint32 _z ) {
		mZ = _z;
		return *this;
	}

	inline Coordinate& Coordinate::operator= (const Coordinate& _o ) {
		mX = _o.mX;
		mY = _o.mY;
		mZ = _o.mZ;
		return *this;
	}

	inline Coordinate& Coordinate::operator= (const QPoint& _p ) {
		mX = _p.x();
		mY = _p.y();
		return *this;
	}

	inline Coordinate& Coordinate::operator+= (const Coordinate& _s ) {
		mX += _s.mX;
		mY += _s.mY;
		mZ += _s.mZ;
		return *this;
	}
	inline	Coordinate& Coordinate::operator+= (const QPoint& _o ) {
		mX += _o.x();
		mY += _o.y();
		return *this;
	}

	inline bool Coordinate::operator <(const Coordinate& _o ) const {
		return (mX<_o.mX||
				(mX==_o.mX&&mY<_o.mY)||
				(mX==_o.mX&&mY==_o.mY&&mZ<_o.mZ)) ? true : false;
	}

	inline Coordinate::operator QString() const {
		return QString("(%1|%2|%3)").arg(mX).arg(mY).arg(mZ);
	}

	inline QPointF Coordinate::toScenePoint() const {
		return QPointF( 22.0f*(mX-mY),
				22.0f*(mX+mY)-4.0f*mZ );
	}

	inline qreal Coordinate::toSceneZValue() const {
		return mZ +
		mX*4.0f +
		mY*4.0f;
	}
}

Q_DECLARE_METATYPE(game::Coordinate);


#endif /* COORDINATE_HPP_ */
