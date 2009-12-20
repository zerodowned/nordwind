#ifndef TEXTURES_H
#define TEXTURES_H
#include "IndexFile.hpp"
#include "Hues.hpp"

namespace resource {
        class Textures : public IndexFile {
		public:
			Textures( QString _indexFIle, QString _dataFile, QObject* _parent );
			QPixmap getEntry( ID _id, Hue _hue );
		protected:
			QPixmap decode( quint16 _width, quint16 _height, QByteArray _data, Hue _hue );
	};
}
#endif
