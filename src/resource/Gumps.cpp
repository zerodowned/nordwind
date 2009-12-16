#include "resource/Gumps.hpp"
#include "resource/Cache.hpp"

using namespace resource;

Gumps::Gumps( QString _indexFile, QString _dataFile, QObject* _parent )
: IndexFile(_indexFile,_dataFile,_parent) {
}

Gumps::~Gumps() {
}

QSharedPointer<Gumps::Entry> Gumps::getEntry( ID _id, QSharedPointer<Hues::Entry> _hue ) {
	QByteArray key = QByteArray::number(Object::Gumps) + QByteArray::number(_id) + QByteArray::number(_hue->getID());
	QSharedPointer<Gumps::Entry> result = Cache::instance().lookup<Gumps::Entry>(key);
	if(result.isNull()) {
		QByteArray data = getData(_id);
		if(!data.isEmpty()) {
			quint16 width = mEntries[_id].getWidth();
			quint16 height = mEntries[_id].getHeight();
			result = Cache::instance().manage<Gumps::Entry>( new Gumps::Entry( _id, decodeGump( width, height, data, _hue ), _hue ) );
		}
	}
	return result;
}

Image Gumps::decodeGump( quint16 _width, quint16 _height, QByteArray _data, Hue _hue ) {
	Q_ASSERT_X(_width>0&&_height>0, __PRETTY_FUNCTION__,"Corrupt entry.");

	QImage image(_width,_height,QImage::Format_ARGB32);
	image.fill(0x00000000);

	QDataStream stream(_data);
	// Read the lookup table
	QVector<qint32> lookupTable( _height );
	stream.readRawData( (char*)lookupTable.data(), _height<<4 );


	for( quint16 y = 0; y < lookupTable.size(); y++ ) {
		// seek in stream to start position in line y
		stream.device()->seek(lookupTable[y]<<2);

		quint16 x = 0;
		while( x < _width ) {
			Colour16 colour;
			quint16 length;
			stream >> colour >> length;
			if(!colour) {
				x += length;
				continue;
			}
			quint16 xend = qMax<quint16>( x+length, _width );
			Colour pixel = _hue->mapToHue(colour);
			while( x < xend ) {
				image.setPixel( x,y, pixel );
			}
		}
	}
	return Image(new QPixmap(QPixmap::fromImage(image)));
}
