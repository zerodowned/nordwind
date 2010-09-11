/*
 * MulFileEngine.cpp
 *
 *  Created on: Sep 6, 2010
 *      Author: idstein
 */

#include "MulFileEngine.hpp"
#include "Index.hpp"

using namespace resource;

MulFileEngine::MulFileEngine(const QString& mulFile, const quint32& identifier) :
	QFSFileEngine(mulFile), mIdentifier(identifier), mIndexHandle(
			IndexFileHandle::instance(mulFile)) {
}

MulFileEngine::~MulFileEngine() {
}

bool MulFileEngine::open(QIODevice::OpenMode mode) {
	bool isOpen = QFSFileEngine::open(mode);
	if (isOpen && mIndexHandle && !(mIndexHandle->value(mIdentifier).isValid() && QFSFileEngine::seek(mIndexHandle->value(mIdentifier).mOffset))) {
			isOpen = false;
			QFSFileEngine::close();
	}
	return isOpen;
}

qint64 MulFileEngine::pos() const {
	qint64 pos = QFSFileEngine::pos();
	if (mIndexHandle)
		pos -= mIndexHandle->value(mIdentifier).mOffset;
	return pos;
}

bool MulFileEngine::seek(qint64 pos) {
	if(mIndexHandle)
		pos += mIndexHandle->value(mIdentifier).mOffset;
	return QFSFileEngine::seek(pos);
}

qint64 MulFileEngine::size() const {
	if (mIndexHandle) {
		return mIndexHandle->value(mIdentifier).mSize;
	} else {
		return QFSFileEngine::size();
	}
}

QString MulFileEngine::fileName(QAbstractFileEngine::FileName file) const {
	QString fileName = QFSFileEngine::fileName(file);
	switch(file) {
		case QAbstractFileEngine::BaseName:
		case QAbstractFileEngine::AbsoluteName:
		case QAbstractFileEngine::DefaultName:
			fileName.append(QString(":%1").arg(mIdentifier));
			break;
		default:
			break;
	}
	return fileName;
}

void MulFileEngine::setFileName( const QString & file ) {
	QFSFileEngine::setFileName(file.section(':',0,-2));
	mIdentifier = file.section(':',-1,-1).toUInt();
}

QAbstractFileEngine* MulFileEngineHandler::create(const QString& fileName) const {
	return QDir::match("*.mul:*", fileName) ? new MulFileEngine(
			fileName.section(':', -2, -2),
			fileName.section(':', -1, -1).toUInt()) : 0;
}

bool MulFileEngine::supportsExtension(QAbstractFileEngine::Extension extension) const {
	switch(extension) {
		case Index::IndexExtension:
			return true;
		default:
			return QFSFileEngine::supportsExtension(extension);
	}
}

bool MulFileEngine::extension(QAbstractFileEngine::Extension extension,
		const QAbstractFileEngine::ExtensionOption *option,
		QAbstractFileEngine::ExtensionReturn *output) {
	switch(extension) {
		case Index::IndexExtension:
			if(mIndexHandle) {
				Index* returnValue = static_cast<Index*>(output);
				*returnValue = mIndexHandle->value(mIdentifier);
				return returnValue->isValid();
			} else
				return false;
		default:
			return QFSFileEngine::extension(extension,option,output);
	}
}

