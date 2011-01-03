#ifndef CCURSOR_H
#define CCURSOR_H

#include <Qt/QCursor.h>
#include <Qt/QVector.h>

namespace gui {
	/*
		This class manages the software cursor.
	*/
	class Cursor : public QObject {
		protected:
			QVector<QCursor> mCursors;
			QVector<QCursor> mCursorsWar;
		public:
			// This enumeration is only used here. Hence it's not exported to
			// enums.h
			enum Type {
				NORTHWEST = 0,
				NORTH,
				NORTHEAST,
				EAST,
				SOUTHEAST,
				SOUTH,
				SOUTHWEST,
				WEST,
				GRAB,
				STANDART,
				PICK,
				UNKWN, // What is this for?
				TARGET,
				WAIT,
				WRITE,
				PIN,
				COUNT
			};
			Cursor();
			bool load();
			bool unload();
			bool reload();
			QCursor& operator[] ( Type _type );
			QCursor& getCursor( Type _cursor, bool _warmode = false );
			//search by string
			//QCursor& getCursor( QString _cursor, bool _warmode = false );
	};
}

#endif
