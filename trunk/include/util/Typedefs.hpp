/*
 * typedefs.h
 *
 *  Created on: 13.12.2009
 *      Author: idstein
 */

#ifndef TYPEDEFS_HPP_
#define TYPEDEFS_HPP_

#include <qglobal.h>
#include <qpair.h>
#include <qpixmap.h>
#include <qrgb.h>
#include <qsharedpointer.h>
#include <qvector.h>

	typedef quint32 ID;
	typedef ID Body;
	typedef qint8 Z;
	typedef quint16 Colour16;
	typedef QRgb Colour;
	enum Direction {
		West = 0,
		Right = 1,
		East = 2,
		Down = 3,
		South = 4,
		Left = 5,
		North = 6,
		Up = 7
	};
	typedef quint8 Action;
	typedef QPoint Center;
	typedef QPair<Center, QPixmap> Frame;
	typedef QVector<Frame> Sequence;
	typedef QHash<Direction, Sequence> Directions;
	typedef QHash<Action, Directions> Actions;


#endif /* TYPEDEFS_H_ */
