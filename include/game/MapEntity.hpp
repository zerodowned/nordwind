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
#include "../resource/Facets.hpp"

namespace game {

	class MapEntity : public Entity {
		public:
			MapEntity( QPoint _position, resource::Facets::MapTile& _data,
				const resource::Facets::MapTile& _south,
				const resource::Facets::MapTile& _down,
				const resource::Facets::MapTile& _east );
			void paint( QPainter * painter, const QStyleOptionGraphicsItem * option,QWidget * widget = 0 );
		protected:
                        quint16 calculateStrech( qint16 _z1, qint16 _z2 ) const;
		private:
			resource::Facets::MapTile& mData;
			QSharedPointer<resource::Land> mLand;
                       quint16 mHl;
                        quint16 mHr;
                        quint16 mStl;
                        quint16 mStr;
	};
}


#endif /* MAPENTITY_HPP_ */
