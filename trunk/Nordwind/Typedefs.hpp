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

typedef quint32 Serial;

enum Privileg {
        Player		=	1 << 0,
        Gamemaster	=	1 << 1,
        Designer	=	1 << 2,
        Developer	=	1 << 3,
        God		=	1 << 4
};

enum Feature {
        // 0xb9 Features
        T2A = 0x0001 | 0x0004, // T2A complete
        LBR = 0x0008 | 0x0002, // LBR + Sound
        AOS = 0x0010, // AOS

        // Characterlist Features
        CONTEXTMENUS = 0x08,
        TOOLTIPS = 0x20,
        AOSPROFESSIONS = 0x20,
        SEPROFESSIONS = 0x80,
        SIXTHCHARSLOT = 0x40,
        ONECHARSLOT = 0x14,
};

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
