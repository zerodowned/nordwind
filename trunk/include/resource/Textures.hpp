#ifndef TEXTURES_H
#define TEXTURES_H
#include "IndexFile.hpp"
#include "Texture.hpp"

namespace resource {
	class Textures : protected IndexFile {
		public:
			class Entry : protected Texture {
				public:
					Entry( ID _id, Image _image, QSharedPointer<Hues::Entry> _hue );
					virtual ~Entry();
			};
			Textures( QString _indexFIle, QString _dataFile, QObject* _parent );
			virtual ~Textures();
			QSharedPointer<Entry> getEntry( ID _id, QSharedPointer<Hues::Entry> _hue );
		protected:
			Image decode( quint16 _width, quint16 _height, QByteArray _data, QSharedPointer<Hues::Entry> _hue );
	};

	inline Textures::Entry::Entry( ID _id, Image _image, QSharedPointer<Hues::Entry> _hue )
	: Texture( Object::Textures, _id, _image, _hue ) {
	}

	inline Textures::Entry::~Entry() {
	}
}
#endif
