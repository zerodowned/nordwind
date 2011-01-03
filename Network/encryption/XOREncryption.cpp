/*
 * XOREncryption.cpp
 *
 *  Created on: Oct 30, 2010
 *      Author: idstein
 */

#include "XOREncryption.hpp"

XOREncryption::XOREncryption(unsigned int seed, unsigned int key1,
		unsigned int key2) :
	mKey1(key1), mKey2(key2), mTable1((((~seed) ^ 0x00001357) << 16) | ((seed
			^ 0xffffaaaa) & 0x0000ffff)), mTable2(((seed ^ 0x43210000) >> 16)
			| (((~seed) ^ 0xabcdffff) & 0xffff0000)) {
}

QByteArray XOREncryption::out(QByteArray& data) {
        QByteArray unencrypted;
	char* pos = data.begin();
        register unsigned int eax, ecx, edx, esi;
	for (const char* end = pos + data.length(); pos != end; pos++) {
                unencrypted.append(*pos ^ (char) (mTable1 & 0xFF));
		edx = mTable2;
		esi = mTable1 << 31;
		eax = mTable2 >> 1;
		eax |= esi;
		eax ^= mKey1 - 1;
		edx <<= 31;
		eax >>= 1;
		ecx = mTable1 >> 1;
		eax |= esi;
		ecx |= edx;
		eax ^= mKey1;
		ecx ^= mKey2;
		mTable1 = ecx;
		mTable2 = eax;
	}
        data.clear();
        return unencrypted;
}
