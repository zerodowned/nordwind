/*
 * IndexFile.cpp
 *
 *  Created on: 13.12.2009
 *      Author: idstein
 */
#include "IndexFile.hpp"

using namespace resource;

IndexFile::IndexFile(QString _indexFile, QString _dataFile, QObject *_parent)
: QObject(_parent) {
	Q_ASSERT_X( QFile::exists( _indexFile ) && QFile::exists(_dataFile),
				__PRETTY_FUNCTION,
				"File(s) not available! Is the path correct?");
	QFile* indexFile, dataFile;
	indexFile = new QFile( _indexFile, this );
	dataFile = new QFile( _dataFile, this );
	Q_ASSERT_X( indexFile->open( QIODevice::ReadOnly ) && dataFile->open( QIODevice::ReadOnly ),
				__PRETTY_FUNCTION,
				"Unable to open file(s) in read-mode!" );
	mIndexStream.setDevice( indexFile );
	mIndexStream.setByteOrder( QDataStream::LittleEndian );
	mDataStream.setDevice( dataFile );
	mDataStream.setByteOrder( QDataStream::LittleEndian );
	quint32 entry_count = mIndexStream.device()->size()/12;
	for( quint32 i = 0; i < entry_count; i++ ) {
		IndexFile::Entry entry;
		mIndexStream >> entry.mOffset >> entry.mSize >> entry.mExtra;
		if(entry.isValid()) {
			mEntries.insert( i, entry );
		}
	}
}

IndexFile::~IndexFile() {
}

QByteArray IndexFile::getData(ID _id ) {
	QByteArray result;
	if(mEntries.contains(_id)) {
		IndexFile::Entry entry = mEntries[_id];
		mDataStream.device()->seek( entry.mOffset );
		result.resize(entry.mSize);
		Q_ASSERT_X(mDataStream.readRawData(result.data(), entry.mSize),
				__PRETTY_FUNCTION__, "Insufficient bytes to read!");
	}
	return result;
}
