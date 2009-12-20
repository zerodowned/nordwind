#include "resource/Textures.hpp"

using namespace resource;

Textures::Textures( QString _indexFile, QString _dataFile, QObject* _parent )
: IndexFile(_indexFile,_dataFile,_parent) {
}

QPixmap Textures::getEntry( ID _id, Hue _hue ) {
	QPixmap result;
	QByteArray data = getData( _id );
	if(!data.isEmpty()) {
		quint16 width, height;
		if(mEntries[_id].mSize==0x2000) {
			width = height = 64;
		} else {
			width = height = 128;
		}
		result = decode(width,height,data,_hue);
	}
	return result;
}

QPixmap Textures::decode( quint16 _width, quint16 _height, QByteArray _data, Hue _hue ) {
	QDataStream stream(_data);
	stream.setByteOrder(QDataStream::LittleEndian);

	QImage image(_width,_height, QImage::Format_ARGB32_Premultiplied);
	image.fill(0);

	QVector<Colour16> imgData(_width*_height);
	stream.readRawData((char*)imgData.data(),sizeof(Colour16)*imgData.size());

	quint16 x,y;
	for(quint16 c = 0; y < _height; y = c/_width, x = c%_width, c++ ) {
		image.setPixel( x, y, _hue->mapToHue(imgData[c]));
	}
	return QPixmap::fromImage(image);
}
