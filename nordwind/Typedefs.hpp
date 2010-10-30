/*
 * typedefs.h
 *
 *  Created on: 13.12.2009
 *      Author: idstein
 */

#ifndef TYPEDEFS_HPP_
#define TYPEDEFS_HPP_

#include <qglobal.h>
#include <qcolor.h>
#include <qobject.h>
#include <qpointer.h>
#include <qdebug.h>
#include <qapplication.h>
#include <QMetaEnum>

typedef quint32 ID;
typedef ID Hue;
typedef qint8 Z;
typedef quint16 Colour16;
typedef QRgb Colour;

inline quint8 red( Colour16 _colour16 ) {
	return ((_colour16 >> 10) & 0x1F);
}

inline quint8 qRed( Colour16 _colour16 ) {
	return red(_colour16) * 8;
}

inline quint8 green( Colour16 _colour16 ) {
	return ((_colour16 >> 5) & 0x1F);
}

inline quint8 qGreen( Colour16 _colour16 ) {
	return green(_colour16) * 8;
}

inline quint8 blue( Colour16 _colour16 ) {
	return (_colour16 & 0x1F);
}

inline quint8 qBlue( Colour16 _colour16 ) {
	return blue(_colour16) * 8;
}
inline QRgb qRgb( Colour16 _colour16 ) {
	return qRgb( qRed(_colour16), qGreen(_colour16), qBlue(_colour16) );
}

template<typename T>
inline QMetaEnum enumerator(const char* name) {
	return T::staticMetaObject.enumerator(T::staticMetaObject.indexOfEnumerator(name));
}

template <typename T>
class Singleton
{
public:
	static T& instance()
	{
		if(!sInstance)
			qFatal("Invalid instance");
		return *sInstance;
	}
//	static const T& const_instance()
//	{
//		if(!sInstance)
//			qFatal("Invalid instance");
//		return *sInstance;
//	}
	static T* pointer() {
		if(!sInstance)
			qDebug() << "No instance initialized";
		return sInstance;
	}
	static void setPointer(T* p = NULL) {
		if(sInstance) {
			qDebug() << "Resetting instance";
			delete sInstance;
		}
		sInstance = p;
	}
	virtual ~Singleton ()
	{
		sInstance = 0;
	}
private:
	static T* sInstance;
protected:
	Singleton () { }
};
template <typename T> T* Singleton <T>::sInstance = 0;

#endif /* TYPEDEFS_H_ */
