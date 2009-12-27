/*
 * StaticEntity.hpp
 *
 *  Created on: 18.12.2009
 *      Author: idstein
 */

#ifndef STATICENTITY_HPP_
#define STATICENTITY_HPP_

#include "Entity.hpp"
#include "../core/Resources.hpp"
#include "../resource/Facets.hpp"


namespace game {
	class Static : public Entity {
		public:
			Static(Coordinate _position, ID _id, ID _hueID);
		private:
			resource::Tile mData;
	};
}

#endif /* STATICENTITY_HPP_ */
