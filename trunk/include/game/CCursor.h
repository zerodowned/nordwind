#ifndef CCURSOR_H
#define CCURSOR_H

#include <Qt/QCursor.h>
#include <Qt/QVector.h>

namespace gui {
	// This enumeration is only used here. Hence it's not exported to
	// enums.h
	enum eCursorType {
		eCursor_NORTHWEST = 0,
		eCursor_NORTH,
		eCursor_NORTHEAST,
		eCursor_EAST,
		eCursor_SOUTHEAST,
		eCursor_SOUTH,
		eCursor_SOUTHWEST,
		eCursor_WEST,
		eCursor_GRAB,
		eCursor_STANDART,
		eCursor_PICK,
		eCursor_UNKWN, // What is this for?
		eCursor_TARGET,
		eCursor_WAIT,
		eCursor_WRITE,
		eCursor_PIN,
		CURSOR_COUNT
	};
	/*
		This class manages the software cursor.
	*/
	class CCursor {
		protected:
			QVector<QCursor*> m_cursors;
			QVector<QCursor*> m_cursorsWar;
		public:
			CCursor();
			~CCursor();
			bool load();
			bool unload();
			bool reload();
			QCursor& getCursor( eCursorType _cursor, bool _warmode = false );
			//search by string
			//QCursor& getCursor( QString _cursor, bool _warmode = false );
	};
}

#endif
