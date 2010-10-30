/*
 * Dynamic.hpp
 *
 *  Created on: 22.12.2009
 *      Author: idstein
 */

#ifndef DYNAMIC_HPP__
#define DYNAMIC_HPP__

#include <QGraphicsObject>

namespace game {
	class Dynamic : public QGraphicsObject {
		Q_OBJECT;
		Q_PROPERTY(Serial serial READ serial WRITE setSerial);
		public:
			typedef quint32 Serial;
			Dynamic( const Serial& serial = -1 );
			Serial serial() const;
			void setSerial( Serial serial );
		private:
			Serial mSerial;
	};

	inline Dynamic::Dynamic(const Dynamic::Serial& serial)
	: QGraphicsObject(),
	  mSerial(serial) {
	}

	inline Dynamic::Serial Dynamic::serial() const {
		return mSerial;
	}

	inline void Dynamic::setSerial( Serial _serial ) {
		mSerial = _serial;
	}
}

#endif /* DYNAMIC_HPP_ */

