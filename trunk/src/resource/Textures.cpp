#include "resource/Textures.hpp"
#include "resource/Cache.hpp"

using namespace resource;

Textures::Textures( QString _indexFile, QString _dataFile, QObject* _parent )
: IndexFile(_indexFile,_dataFile,_parent) {
}

Textures::~Textures() {
}

QSharedPointer<Textures::Entry> Textures::getEntry( ID _id, QSharedPointer<Hues::Entry> _hue ) {
	QByteArray key = QByteArray::number(Object::Textures) + QByteArray::number(_id) + QByteArray::number(_hue->getID());
	QSharedPointer<Textures::Entry> result = Cache::instance().lookup<Textures::Entry>( key );
	if(result.isNull()) {
		QByteArray data = getData( _id );
		if(!data.isEmpty()) {
			quint16 width, height;
			if(mEntries[_id].mSize==0x2000) {
				width = height = 64;
			} else {
				width = height = 128;
			}
			result = Cache::instance().manage<Textures::Entry>( new Textures::Entry( _id, decode( width, height, data, _hue), _hue) );
		}
	}
	return result;
}

Image Textures::decode( quint16 _width, quint16 _height, QByteArray _data, QSharedPointer<Hues::Entry> _hue ) {
	QDataStream stream(_data);
	stream.setByteOrder(QDataStream::LittleEndian);

	Image image( new QImage(_width,_height, QImage::Format_ARGB32) );
	Q_ASSERT(!image.isNull());
	image->fill(0x00000000);

	QVector<Colour16> imgData( _width*_height );
	stream.readRawData( (char*)imgData.data(), sizeof(Colour16)*imgData.size() );

	for( quint16 y = 0; y < _height; y ++ ) {
		for( quint16 x = 0; x < _width; x++ ) {
			image->setPixel( x, y, _hue->mapToHue(imgData[y*_height+x]));
		}
	}
	return image;
}
