/*
 * XOREncryption.hpp
 *
 *  Created on: Oct 30, 2010
 *      Author: idstein
 */

#ifndef XORENCRYPTION_HPP_
#define XORENCRYPTION_HPP_

#include "../Socket.hpp"
/*
 *
 */
namespace network {

class XOREncryption : public Socket::Operator {
public:
	XOREncryption(unsigned int seed, unsigned int key1, unsigned int key2);
	void out(QByteArray& data);
private:
	unsigned int mKey1;
	unsigned int mKey2;
	unsigned int mTable1;
	unsigned int mTable2;
};

}

#endif /* XORENCRYPTION_HPP_ */
