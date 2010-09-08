/*
 * util.hpp
 *
 *  Created on: 14.12.2009
 *      Author: idstein
 */

#ifndef UTIL_HPP_
#define UTIL_HPP_

#include "Typedefs.hpp"
#include <qdatastream.h>

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

template <typename T>
QDataStream &operator<<(QDataStream& _dataStream, const T& _obj ) {
	return _obj.write(_dataStream);
}

template <typename T>
QDataStream &operator>>(QDataStream& _dataStream, T& _obj ) {
	return _obj.read( _dataStream );
}


#endif /* UTIL_HPP_ */
