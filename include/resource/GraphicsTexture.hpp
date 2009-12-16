/*
 * Texture.hpp
 *
 *  Created on: 13.12.2009
 *      Author: idstein
 */

#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

#include "Object.hpp"
#include "Hues.hpp"

namespace resource {
	class GraphicsTexture : public Object {
		public:
			GraphicsTexture( Object::Type _type, ID _id, Image _image, Hue _hue );
			virtual ~GraphicsTexture();
			Hue getHue() const;
			ID getID() const;
			Image getImage() const;
		protected:
			ID mID;
			Image mImage;
			Hue mHue;
	};

	inline GraphicsTexture::GraphicsTexture( Object::Type _type, ID _id, Image _image, Hue _hue )
	: Object(_type, QByteArray::number(_type) + QByteArray::number(_id) ),
	  mID(_id),
	  mImage(_image),
	  mHue(_hue) {
	}

	inline GraphicsTexture::~GraphicsTexture() {
	}

	inline ID GraphicsTexture::getID() const {
		return mID;
	}

	inline Hue GraphicsTexture::getHue() const {
		return mHue;
	}

	inline Image GraphicsTexture::getImage() const {
		return mImage;
	}
}

#endif /* TEXTURE_HPP_ */
