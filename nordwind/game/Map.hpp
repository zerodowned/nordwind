/*
 * MapEntity.hpp
 *
 *  Created on: 16.12.2009
 *      Author: idstein
 */
#ifndef MAPENTITY_HPP_
#define MAPENTITY_HPP_

#include <qgraphicsitem.h>
#include "../Typedefs.hpp"

namespace game {
	class Map : public QGraphicsItem {
		public:
			Map(ID id = 0x2, Z z = 0, Z southZ = 0, Z eastZ = 0, Z downZ = 0);
			virtual ~Map();
			virtual QRectF boundingRect () const;
			virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
			virtual void setZValue(qreal z);
		private:
			ID mID;
			Z mZ;
			QPixmap mTexture;
			Z mSouthZ;
			Z mEastZ;
			Z mDownZ;
	};
}

inline void game::Map::setZValue(qreal z) {
	setZValue(z-4.0f);
}


#endif /* MAPENTITY_HPP_ */
