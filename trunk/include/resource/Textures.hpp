#ifndef TEXTURES_H
#define TEXTURES_H
#include "IndexFile.hpp"
#include "Texture.hpp"

namespace resource {
	class Textures : protected IndexFile {
		public:
			class Entry : public Texture {
				public:
					Entry( ID _id, Image _image, Hue _hue );
					virtual ~Entry();
			};
			Textures( QString _indexFIle, QString _dataFile, QObject* _parent );
			virtual ~Textures();
			QSharedPointer<Entry> getEntry( ID _id, Hue _hue );
		protected:
			Image decode( quint16 _width, quint16 _height, QByteArray _data, Hue _hue );
	};

	inline Textures::Entry::Entry( ID _id, Image _image, Hue _hue )
	: Texture( Object::Textures, _id, _image, _hue ) {
	}

	inline Textures::Entry::~Entry() {
	}
}
#endif
