/*
 * StaticEntity.hpp
 *
 *  Created on: 18.12.2009
 *      Author: idstein
 */

#ifndef STATICENTITY_HPP_
#define STATICENTITY_HPP_

#include <QGraphicsPixmapItem>
#include "../Typedefs.hpp"

namespace game {
	class Static : public QGraphicsPixmapItem {
		public:
			Static( ID id, Z z);
			ID id() const;
//			Hue hue() const;
		private:
			ID mID;
			Z mZ;
	};

	inline ID Static::id() const {
		return mID;
	}

//	inline Hue Static::hue() const {
//		return mHue;
//	}
}

#endif /* STATICENTITY_HPP_ */
