/*
 * IndexFileHandle.cpp
 *
 *  Created on: Sep 8, 2010
 *      Author: idstein
 */

#include "IndexFileHandle.hpp"
#include <qdebug.h>
#include <qdatastream.h>

using namespace resource;

QHash<QString, QSharedPointer<IndexFileHandle> > IndexFileHandle::mInstances;

QSharedPointer<IndexFileHandle> IndexFileHandle::instance(
		const QString& mulFile, const QString& indexFile) {
	if (!mInstances.contains(mulFile))
		new IndexFileHandle(mulFile, indexFile);
	return mInstances[mulFile];
}

IndexFileHandle::IndexFileHandle(const QString& mulFile,
		const QString& indexFile) {
	QFile indexFileHandle(indexFile);
	qDebug() << "Index build of " << mulFile << " from " << indexFile;
	if (indexFileHandle.open(QIODevice::ReadOnly)) {
		qDebug() << "Index build started";
		QDataStream indexStream(indexFileHandle.readAll());
		indexStream.setByteOrder(QDataStream::LittleEndian);
		for (quint32 i = 0; i < indexStream.device()->size() / 12; i++) {
			Index entry;
			indexStream >> entry.mOffset >> entry.mSize >> entry.mExtra;
			if (entry.isValid())
				insert(i, entry);
		}
		qDebug() << count() << " indices read.";
	} else
		qWarning() << indexFileHandle.errorString();
	mInstances.insert(mulFile, QSharedPointer<IndexFileHandle> (this));
}

IndexFileHandle::~IndexFileHandle() {
}
