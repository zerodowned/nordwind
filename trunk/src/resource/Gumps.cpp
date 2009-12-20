#include "resource/Gumps.hpp"

using namespace resource;

Gumps::Gumps( QString _indexFile, QString _dataFile, QObject* _parent )
: IndexFile(_indexFile,_dataFile,_parent) {
}

QPixmap Gumps::getEntry( ID _id, Hue _hue ) {
	QPixmap result;
	QByteArray data = getData(_id);
	if(!data.isEmpty()) {
		quint16 width = mEntries[_id].getWidth();
		quint16 height = mEntries[_id].getHeight();
		result = decodeGump(width,height,data,_hue);
	}
	return result;
}

QPixmap Gumps::decodeGump(quint16 _width,quint16 _height,QByteArray _data,Hue _hue ) {
	Q_ASSERT_X(_width>0&&_height>0, __PRETTY_FUNCTION__,"Corrupt entry.");

	QImage image(_width,_height,QImage::Format_ARGB32_Premultiplied);
	image.fill(0);

	QDataStream stream(_data);
	// Read the lookup table
	QVector<qint32> lookupTable(_height);
	stream.readRawData((char*)lookupTable.data(),_height<<4);


	for(quint16 y = 0;y < lookupTable.size();y++ ) {
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
			Colour pixel = _hue->mapToHue(colour);
			for(; length > 0 && x < _width; x++, length-- )
				image.setPixel( x,y, pixel );
		}
	}
	return QPixmap::fromImage(image);
}
