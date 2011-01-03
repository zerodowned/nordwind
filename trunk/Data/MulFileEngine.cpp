/*
 * MulFileEngine.cpp
 *
 *  Created on: Sep 6, 2010
 *      Author: idstein
 */

#include "MulFileEngine.hpp"

const QChar MulFileEngine::separator = ':';

MulFileEngine::MulFileEngine(const QString& mulFile, const QString& indexFile, const quint32& id) :
        QFSFileEngine(mulFile), mID(id) {
    QFile indexFileHandle(indexFile);
    if(indexFileHandle.open(QIODevice::ReadOnly)
        &&indexFileHandle.seek(id*12)) {
            QDataStream stream(&indexFileHandle);
            stream.setByteOrder(QDataStream::LittleEndian);
            stream >> mIndex.mOffset >> mIndex.mSize >> mIndex.mExtra;
        }
}

MulFileEngine::~MulFileEngine() {
}

bool MulFileEngine::open(QIODevice::OpenMode mode) {
        bool isOpen = QFSFileEngine::open(mode);
        if (isOpen && !(mIndex.isValid() && QFSFileEngine::seek(mIndex.mOffset))) {
                        isOpen = false;
                        QFSFileEngine::close();
        }
        return isOpen;
}

qint64 MulFileEngine::pos() const {
        qint64 pos = QFSFileEngine::pos() - mIndex.mOffset;
        return pos;
}

bool MulFileEngine::seek(qint64 pos) {
        pos += mIndex.mOffset;
        return QFSFileEngine::seek(pos);
}

qint64 MulFileEngine::size() const {
        if (mIndex.isValid())
                return mIndex.mSize;
        else
                return QFSFileEngine::size();
}

QString MulFileEngine::fileName(QAbstractFileEngine::FileName file) const {
        QString fileName = QFSFileEngine::fileName(file);
        switch(file) {
                case QAbstractFileEngine::BaseName:
                case QAbstractFileEngine::AbsoluteName:
                case QAbstractFileEngine::DefaultName:
                        fileName.append(QString(":%1").arg(mID));
                        break;
                default:
                        break;
        }
        return fileName;
}

void MulFileEngine::setFileName( const QString & file ) {
        qFatal("Not yet supported");
        //QFSFileEngine::setFileName(file.section(':',0,-2));
        //mIdentifier = file.section(':',-1,-1).toUInt();
}

bool MulFileEngine::supportsExtension(QAbstractFileEngine::Extension extension) const {
        if (extension == Index::IndexExtension)
                return true;
        else
                return QFSFileEngine::supportsExtension(extension);
}

bool MulFileEngine::extension(QAbstractFileEngine::Extension extension,
                const QAbstractFileEngine::ExtensionOption *option,
                QAbstractFileEngine::ExtensionReturn *output) {
        if(extension==Index::IndexExtension) {
                        if(mIndex.isValid()) {
                            memcpy(output,(void*)(&mIndex),sizeof(mIndex));
                                return true;
                        } else
                                return false;
        } else
                return QFSFileEngine::extension(extension,option,output);
}
