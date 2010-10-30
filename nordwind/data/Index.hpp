/*
 * Index.hpp
 *
 *  Created on: Sep 8, 2010
 *      Author: idstein
 */

#ifndef INDEX_HPP_
#define INDEX_HPP_

#include <qabstractfileengine.h>

/*
 *
 */
namespace data {



class Index: public QAbstractFileEngine::ExtensionReturn {
public:
	enum Extension {
		IndexExtension = QAbstractFileEngine::UnMapExtension+1,
	};
	Index();
	Index& operator=(const Index& other);
	Index(const Index& o);
	quint32 mOffset;
	quint32 mSize;
	quint32 mExtra;
	bool isValid() const;
	quint16 getWidth() const;
	quint16 getHeight() const;
};

inline Index::Index() :
	mOffset(0xFFFFFFFF), mSize(0xFFFFFFFF), mExtra(0) {
}

inline Index::Index(const Index& o) :
	mOffset(o.mOffset), mSize(o.mSize), mExtra(o.mExtra) {
}

inline Index& Index::operator=(const Index& o) {
	mOffset = o.mOffset;
	mSize = o.mSize;
	mExtra = o.mExtra;
	return *this;
}

inline bool Index::isValid() const {
	return mOffset != 0xFFFFFFFF && mSize != 0xFFFFFFFF && mSize != 0 ? true
			: false;
}

inline quint16 Index::getWidth() const {
	return mExtra >> 16;
}

inline quint16 Index::getHeight() const {
	return mExtra & 0xFF;
}

}

#endif /* INDEX_HPP_ */
