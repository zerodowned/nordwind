/*
 * IndexFileHandle.hpp
 *
 *  Created on: Sep 8, 2010
 *      Author: idstein
 */

#ifndef INDEXFILEHANDLE_HPP_
#define INDEXFILEHANDLE_HPP_

#include <qsharedpointer.h>
#include <qhash.h>
#include "Index.hpp"

/*
 *
 */
namespace resource {

class IndexFileHandle: public QHash<quint32, Index> {
public:
	static QSharedPointer<IndexFileHandle> instance(const QString& mulFile,
			const QString& indexFile = QString());
	virtual ~IndexFileHandle();
	explicit IndexFileHandle(const QString& mulFile, const QString& indexFile);
private:
	static QHash<QString, QSharedPointer<IndexFileHandle> > mInstances;
};

}

#endif /* INDEXFILEHANDLE_HPP_ */
