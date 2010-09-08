/*
 * MapEntity.hpp
 *
 *  Created on: 16.12.2009
 *      Author: idstein
 */
#ifndef MAPENTITY_HPP_
#define MAPENTITY_HPP_

#include "Entity.hpp"
#include "Typedefs.hpp"

namespace game {

	class Map : public Entity {
		public:
			Map( Coordinate _position, ID _id, Z _southZ, Z _downZ, Z _eastZ );
//			void paint( QPainter * painter, const QStyleOptionGraphicsItem * option,QWidget * widget = 0 );
			operator QString() const;
		protected:
			qint16 calculateStrech( qint16 _z1, qint16 _z2 ) const;
		private:
			ID mID;
	};

	inline qint16 Map::calculateStrech( qint16 _z1, qint16 _z2 ) const {
		return (_z1-_z2)*4+22;
	}
}


#endif /* MAPENTITY_HPP_ */
