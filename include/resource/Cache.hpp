/*
 * ResourceCache.hpp
 *
 *  Created on: 13.12.2009
 *      Author: idstein
 */

#ifndef CACHE_HPP_
#define CACHE_HPP_

#include <qsharedpointer.h>
#include <qbytearray.h>
#include <qcache.h>
#include <qhash.h>
#include "../util/Singleton.hpp"
#include "Object.hpp"

namespace resource {
	class Cache : public Singleton<Cache> {
		friend class Singleton<Cache>;
		public:
			template<typename T>
			QSharedPointer<T> lookup( QByteArray _key );
			template<typename T, typename KEY>
			QSharedPointer<T> lookup( KEY _key );
			template<typename T>
			QSharedPointer<T> manage( T* object );
			void moveToCache( QByteArray _key, Object* _object );
		protected:
			QSharedPointer<Object> getPointer( QByteArray _key );
		private:
			QHash< QByteArray, QWeakPointer<Object> > mObjects;
			QCache< QByteArray, Object > mCache;
	};

	QSharedPointer<Object> Cache::getPointer( QByteArray _key ) {
		QSharedPointer<Object> result;
		if(mObjects.contains(_key)) {
			result = QSharedPointer<Object>(mObjects[_key],&Object::cache);
		} else {
			if(mCache.contains(_key)) {
				result = QSharedPointer<Object>(mCache.take(_key),&Object::cache);
				mObjects[_key] = result.toWeakRef();
			}
		}
		return result;
	}

	template <typename T>
	inline QSharedPointer<T> Cache::lookup( QByteArray _key ) {
		return getPointer(_key).dynamicCast();
	}

	template <typename T, typename KEY>
	inline QSharedPointer<T> Cache::lookup( KEY _key ) {
		QByteArray key(QByteArray::fromRawData(static_cast<const char*>(&_key),sizeof(KEY)));
		return lookup(key);
	}

	template <typename T>
	inline QSharedPointer<T> Cache::manage( T* _object ) {
		QSharedPointer<T> result = _object;
		// check if T is derivative of Object
		QSharedPointer<Object> tmp = result.dynamicCast();
		if(!tmp.isNull()) {
			mObjects[_object->key()] = tmp.toWeakRef();
			result = QSharedPointer<T>( _object, &T::cache);
		}
		return result;
	}

	inline void Cache::moveToCache( QByteArray _key, Cache::Object* _object ) {
		mObjects.remove(_key);
		mCache.insert(_key, _object);
	}
}

#endif /* RESOURCECACHE_HPP_ */
