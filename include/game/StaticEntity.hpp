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
	class StaticEntity : public Entity {
		public:
			StaticEntity( QPoint _offset, resource::Facets::StaticTile& _rawData );
		private:
			resource::Facets::StaticTile mRawData;
			QSharedPointer<resource::Tile> mTile;
	};
}

#endif /* STATICENTITY_HPP_ */
