#include "resource/Hues.hpp"
#include <qfile.h>
#include <qdebug.h>

using namespace resource;

Hues::Hues( QString _dataFile, QObject* _parent )
: QObject(_parent) {
	Q_ASSERT_X(QFile::exists(_dataFile),__PRETTY_FUNCTION__, _dataFile.toAscii().constData());
	mDataStream.setDevice( new QFile(_dataFile,this) );
	Q_ASSERT_X(mDataStream.device()->open(QIODevice::ReadOnly),__PRETTY_FUNCTION__, _dataFile.toAscii().constData());
	mDataStream.setByteOrder(QDataStream::LittleEndian);
	load();
};

Hues::~Hues() {
}

Hues& Hues::load() {
	quint32 entries = mDataStream.device()->size()/708 * 8;
	quint32 i = 1; // Current hue id
	quint16 group = 0;
	while(!mDataStream.atEnd()&&i<entries) {
		quint32 header = 0;
		mDataStream >> header; // Skip 4 byte header

		// Read 8 hues
		for (unsigned int j = 0; j < 8; ++j) {
			QSharedPointer<Hues::RawHue> rawHue( new RawHue );
			rawHue->mID = i;
			Colour16 color; // Temporary variable for 16-bit color values
			rawHue->mColourTable.resize( 32 );
			// Read 32 colors
			for (quint8 k = 0; k < 32; ++k) {
				mDataStream >> color;
				// Save the r,g,b components
				rawHue->mColourTable[k] = qRgb(color);
			}

			// Read Data Start and Data End
			mDataStream >> rawHue->mStartColour;
			mDataStream >> rawHue->mEndColour;

			char name[20] = {0, };
			mDataStream.readRawData(&name[0], 20);
			rawHue->mName = name;
			mHues.push_back( rawHue );
			++i;
		}
		++group;
	}
	return *this;
}
