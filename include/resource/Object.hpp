/*
 * Object.hpp
 *
 *  Created on: 14.12.2009
 *      Author: idstein
 */

#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include <qbytearray.h>

namespace resource {

	class Object {
		public:
			enum Type {
				Animations,
				Arts,
				Gumps,
				Facet,
				Textures
			};
			Object( Type _type, QByteArray _key );
			virtual ~Object();
			virtual void cache();
			Type getType() const;
			QByteArray getKey() const;
		protected:
			Type mType;
			QByteArray mKey;
	};

	inline Object::Type Object::getType() const {
		return mType;
	}

	inline QByteArray Object::getKey() const {
		return mKey;
	}
}
#endif /* OBJECT_HPP_ */
