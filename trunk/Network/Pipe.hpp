/*
 * Pipe.hpp
 *
 *  Created on: Oct 30, 2010
 *      Author: idstein
 */

#ifndef PIPE_HPP_
#define PIPE_HPP_

#include <qbytearray.h>

/*
 *
 */
class Pipe {
public:
    virtual ~Pipe() {
	}
	virtual QByteArray in(QByteArray& inbound) {
		return inbound;
	}
	virtual QByteArray out(QByteArray& outbound) {
		return outbound;
	}
};

#endif /* COMPRESSION_HPP_ */
