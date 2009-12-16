/*
 * Cache.cpp
 *
 *  Created on: 16.12.2009
 *      Author: idstein
 */
#include "resource/Cache.hpp"

using namespace resource;

QSharedPointer<Object> Cache::getPointer(const QByteArray& _key) {
	QSharedPointer<Object> result;
	if(mObjects.contains(_key)) {
		result = QSharedPointer<Object> (mObjects[_key].data(), &Object::cache);
	} else {
		if (mCache.contains(_key)) {
			result = QSharedPointer<Object> (mCache.take(_key), &Object::cache);
			mObjects[_key] = result.toWeakRef();
		}
	}
	return result;
}

void Cache::moveToCache( const QByteArray& _key, Object* _object ) {
	mObjects.remove(_key);
	mCache.insert(_key, _object);
}
