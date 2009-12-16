/*
 * MapEntity.hpp
 *
 *  Created on: 16.12.2009
 *      Author: idstein
 */
#ifndef MAPENTITY_HPP_
#define MAPENTITY_HPP_

#include "Entity.hpp"
#include "../core/Resources.hpp"

namespace game {
	class MapEntity : public Entity {
		public:
			MapEntity( QPoint _offset, quint8 _x, quint8 _y, resource::Facets::MapTile& _data );
			virtual ~MapEntity();
			void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
		private:
//			QSharedPointer<MapEntity> mSouth;
//			QSharedPointer<MapEntity> mEast;
//			QSharedPointer<MapEntity> mDown;
			resource::Facets::MapTile& mData;
			resource::Art mArt;
			resource::LandInfo mInfo;
			resource::Texture mTexture;
	};
}


#endif /* MAPENTITY_HPP_ */
