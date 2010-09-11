/*
 * StaticEntity.hpp
 *
 *  Created on: 18.12.2009
 *      Author: idstein
 */

#ifndef STATICENTITY_HPP_
#define STATICENTITY_HPP_

#include "Entity.hpp"
#include "../Typedefs.hpp"

namespace game {
	class Static : public Entity {
		public:
			Static(Coordinate _position, ID id, Hue hue);
			ID id() const;
			Hue hue() const;
			void setPixmap(const QPixmap &pixmap);
		private:
			ID mID;
			Hue mHue;
	};

	inline ID Static::id() const {
		return mID;
	}

	inline Hue Static::hue() const {
		return mHue;
	}
}

#endif /* STATICENTITY_HPP_ */
