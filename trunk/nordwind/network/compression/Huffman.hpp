////////////////////////////////////////////////////////////////////////////////
//
// uo_huffman.h
//
// Copyright (C) 2001 Luke 'Infidel' Dunstan
//
// Compression code based on UOX:
// Copyright (C) 1999 Bruno 'Beosil' Heidelberger
//
// Decompression code based on:
// Ultimate Melange. Client for UO-Emulators
// Copyright (C) 2000 Axel Kittenberger
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//
//  Declaration of the compression/decompression classes
//
////////////////////////////////////////////////////////////////////////////////

#ifndef HUFFMAN_HPP_
#define HUFFMAN_HPP_

#include "../Socket.hpp"

namespace network {
	class Huffman : public Socket::Operator {
	public:
		Huffman();
		void in(QByteArray& data);
	private:
		QByteArray uncompress(QByteArray& data);
		static int tree[512];
		// Current bit position 0..7
		int mBitNum;
		// Current position in the tree
		int mTreePos;
		// Byte variable is stored here
		int mValue;
		// Mask for current bit
		int mMask;
	};
}

#endif
