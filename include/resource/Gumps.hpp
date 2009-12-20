#ifndef CGUMPS_H
#define CGUMPS_H

#include "IndexFile.hpp"
#include "Hues.hpp"

namespace resource {
    class Gumps : public IndexFile {
		public:
			Gumps( QString _indexFile, QString _dataFile, QObject* _parent );
			QPixmap getEntry(ID _id, Hue _hue);
		protected:
			QPixmap decodeGump( quint16 _width, quint16 _height, QByteArray _data, Hue _hue );
    };
}

#endif
