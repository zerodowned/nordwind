/*
 * IndexFileHandle.hpp
 *
 *  Created on: Sep 8, 2010
 *      Author: idstein
 */

#ifndef INDEXFILEHANDLE_HPP_
#define INDEXFILEHANDLE_HPP_

#include <qsharedpointer.h>
#include <qvector.h>
#include "Index.hpp"
#include "../Typedefs.hpp"

/*
 *
 */
namespace resource {

class IndexFileHandle: public QVector<Index> {
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
