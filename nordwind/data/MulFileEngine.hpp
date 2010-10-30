/*
 * MulFileEngine.hpp
 *
 *  Created on: Sep 6, 2010
 *      Author: idstein
 */

#ifndef MULFILEENGINE_HPP_
#define MULFILEENGINE_HPP_

#include <qfsfileengine.h>
#include <qsharedpointer.h>
#include "IndexFileHandle.hpp"

namespace data {

class MulFileEngine: public QFSFileEngine {
public:
	MulFileEngine(const QString& mulFile, const quint32& identifier);
	virtual ~MulFileEngine();
	virtual bool open(QIODevice::OpenMode mode);
	virtual qint64 pos() const;
	virtual qint64 size() const;
	virtual bool seek(qint64 pos);
	virtual QString fileName(QAbstractFileEngine::FileName file =
			QAbstractFileEngine::DefaultName) const;
	void	setFileName ( const QString & file );
	virtual bool
			supportsExtension(QAbstractFileEngine::Extension extension) const;
	virtual bool extension(QAbstractFileEngine::Extension extension,
			const QAbstractFileEngine::ExtensionOption * option = 0,
			QAbstractFileEngine::ExtensionReturn * output = 0);
	static const QChar separator;
private:
	quint32 mIdentifier;
	QSharedPointer<IndexFileHandle> mIndexHandle;
};

class MulFileEngineHandler: public QAbstractFileEngineHandler {
public:
	QAbstractFileEngine *create(const QString &fileName) const;
};

}

#endif /* MULFILEENGINE_HPP_ */
