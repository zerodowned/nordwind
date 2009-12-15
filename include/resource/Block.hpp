/*
 * Block.hpp
 *
 *  Created on: 13.12.2009
 *      Author: idstein
 */

#ifndef BLOCK_HPP_
#define BLOCK_HPP_

namespace resource {

	namespace game {

		class Block {
			public:
				struct Map {
					quint16 mID;
					qint8 mZ;
				};
				struct Static {
					quint16 mID;
					quint8 mXOffset;
					quint8 mYOffset;
					qint8 mZ;
					quint16 mColour;
				};
				typedef QVector<Map> MapTiles;
				typedef QVector<Static> StaticTiles;
				Block( ID _id, QPoint _position, MapTiles _mapTiles, StaticTiles _staticTiles );
				virtual ~Block();
				ID getID() const;
				QPoint getPosition() const;
				MapTiles getMapTiles() const;
				StaticTiles getStaticTiles() const;
			private:
				ID mID;
				QPoint mPosition;
				MapTiles mMapTiles;
				StaticTiles mStaticTiles;
		};

		inline ID Block::getID() const {
			return mID;
		}

		inline QPoint Block::getPosition() const {
			return mPosition;
		}

		inline Block::MapTiles Block::getMapTiles() const {
			return mMapTiles;
		}

		inline Block::StaticTiles Block::getStaticTiles() const {
			return mStaticTiles;
		}
	}

}

#endif /* BLOCK_HPP_ */
