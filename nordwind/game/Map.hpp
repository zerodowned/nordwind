/*
 * MapEntity.hpp
 *
 *  Created on: 16.12.2009
 *      Author: idstein
 */
#ifndef MAPENTITY_HPP_
#define MAPENTITY_HPP_

#include "Entity.hpp"
#include "../Typedefs.hpp"
//#include <qfuturewatcher.h>

namespace game {

	class Map : public Entity {
		public:
			Map( const QPoint& p, ID _id, Z _z, Z _southZ, Z _downZ, Z _eastZ );
//			void paint( QPainter * painter, const QStyleOptionGraphicsItem * option,QWidget * widget = 0 );
			operator QString() const;
		protected:
			qint16 calculateStrech( Z _z1, Z _z2 ) const;
		private:
			ID mID;
			//QFutureWatcher<QImage>* mLoading;
	};

	inline qint16 Map::calculateStrech( Z _z1, Z _z2 ) const {
		return qAbs((_z1-_z2)*4+22);
	}
}


#endif /* MAPENTITY_HPP_ */
