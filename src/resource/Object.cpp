/*
 * Object.cpp
 *
 *  Created on: 14.12.2009
 *      Author: idstein
 */
#include "resource/Object.hpp"
#include "resource/Cache.hpp"

using namespace resource;

Object::Object( Object::Type _type, QByteArray _key )
: mType(_type),
  mKey(QByteArray::number(_type) + _key) {
}

Object::~Object() {
}

void Object::cache() {
	Cache::instance().moveToCache( getKey(), this );
}
