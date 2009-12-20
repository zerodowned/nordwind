#include "resource/Hues.hpp"
#include <qfile.h>
#include <qdebug.h>

using namespace resource;

Hues::Hues( QString _dataFile, QObject* _parent )
    : QObject(_parent), mDefaultHue( new Hues::DefaultHue ) {
	Q_ASSERT_X(QFile::exists(_dataFile),__PRETTY_FUNCTION__, _dataFile.toAscii().constData());
	mDataStream.setDevice( new QFile(_dataFile,this) );
	Q_ASSERT_X(mDataStream.device()->open(QIODevice::ReadOnly),__PRETTY_FUNCTION__, _dataFile.toAscii().constData());
	mDataStream.setByteOrder(QDataStream::LittleEndian);
	load();
};

Hues::~Hues() {
}

Hues& Hues::load() {
    QSharedPointer<Hues::RawHue> defaultHue( new RawHue );
    mHues.push_back( defaultHue );
        quint32 i = 0;
	quint16 group = 0;
	while(!mDataStream.atEnd()) {
		if(i%8==0) {
			mDataStream.skipRawData(4); // Skip 4 byte header
			group++;
		}

		// Read 8 hues
		QSharedPointer<Hues::RawHue> rawHue( new RawHue );
		rawHue->mID = i+1;
		rawHue->mColourTable.resize( 32 );
		// Read 32 colors
		Colour16 color; // Temporary variable for 16-bit color values
		for (quint8 k = 0; k < 32; k++) {
			mDataStream >> color;
			// Save the r,g,b components
                        QRgb color32 = qRgb(color);
                        rawHue->mColourTable[k] = color32;
		}

		// Read Data Start and Data End
		quint16 col16;
		mDataStream >> col16;
		rawHue->mStartColour = qRgb(col16);
		mDataStream >> col16;
		rawHue->mEndColour = qRgb(col16);

		char name[20] = {0, };
		mDataStream.readRawData(&name[0], 20);
		rawHue->mName = name;
		mHues.push_back( rawHue );
		i++;
	}
	return *this;
}

Colour Hues::DefaultHue::mapToHue( Colour16 _colour ) const {
	return qRgb(_colour);
}

Colour Hues::PartialHue::mapToHue( Colour16 _colour ) const {
	quint8 r = red(_colour);
	if(r==green(_colour)&&
			r==blue(_colour)) {
		return mRawHue->mColourTable[r];
	} else {
		return qRgb(_colour);
	}
}

Colour Hues::Entry::mapToHue( Colour16 _colour ) const {
	return mRawHue->mColourTable[red(_colour)];
}
