/*
 * Block.cpp
 *
 *  Created on: 13.12.2009
 *      Author: idstein
 */

#include "Block.hpp"

using namespace resource::game;

Block::Block( ID _id, QPoint _position, MapTiles _mapTiles, StaticTiles _staticTiles )
: mID(_id),
  mPosition(_position),
  mMapTiles(_mapTiles),
  mStaticTiles(_staticTiles) {
}

Block::~Block() {
}
