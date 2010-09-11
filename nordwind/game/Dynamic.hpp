/*
 * Dynamic.hpp
 *
 *  Created on: 22.12.2009
 *      Author: idstein
 */

#ifndef DYNAMIC_HPP_
#define DYNAMIC_HPP_

#include <qobject.h>
#include "Entity.hpp"

namespace game {
	typedef quint32 Serial;

	class Dynamic : public QObject, public Entity {
		Q_OBJECT;
		Q_PROPERTY(Serial serial READ getSerial WRITE setSerial);
		public:
			Dynamic( const Coordinate& _position, Serial _serial = -1 );
			Serial serial() const;
			Dynamic& setSerial( Serial _setial );
		private:
			Serial mSerial;
	};

	inline Dynamic::Dynamic(const Coordinate& _position,Serial _serial)
	: QObject(),
	  Entity(_position),
	  mSerial(_serial) {
	}

	inline Serial Dynamic::serial() const {
		return mSerial;
	}

	inline Dynamic& Dynamic::setSerial( Serial _serial ) {
		mSerial = _serial;
		return *this;
	}
}


#endif /* DYNAMIC_HPP_ */
