#ifndef CGUMPS_H
#define CGUMPS_H

#include "IndexFile.hpp"
#include "Texture.hpp"

namespace resource {
    class Gumps : public IndexFile {
		public:
			class Entry : public Texture {
				public:
					Entry( ID _id, Image _image, QSharedPointer<Hues::Entry> _hue );
					virtual ~Entry();
			};
			Gumps( QString _indexFile, QString _dataFile, QObject* _parent );
			virtual ~Gumps();
			QSharedPointer<Entry> getEntry(ID _id, QSharedPointer<Hues::Entry> _hue);
		protected:
			Image decodeGump( quint16 _width, quint16 _height, QByteArray _data, Hue _hue );
    };

    typedef QSharedPointer<Gumps::Entry> Gump;

    inline Gumps::Entry::Entry(ID _id, Image _image, QSharedPointer<Hues::Entry> _hue )
    : Texture( Object::Gumps, _id, _image, _hue ) {
    }

    inline Gumps::Entry::~Entry() {
    }
}

#endif
