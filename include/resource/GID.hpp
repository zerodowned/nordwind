/*
 * GID.hpp
 *
 *  Created on: 19.12.2009
 *      Author: idstein
 */

#ifndef GID_HPP_
#define GID_HPP_

#include "Hues.hpp"

namespace resource {
	class GID {
		public:
			GID( ID _id, Hue _hue );
			GID( const GID& _o );
			bool operator==( const GID& _o ) const;
			ID getID() const;
			Hue getHue() const;
			GID& setHue(Hue _newHue);
			operator QString() const;
		private:
			ID mID;
			Hue mHue;
	};

	inline GID::GID( ID _id, Hue _hue )
	: mID(_id),
	  mHue(_hue) {
	}

	inline GID::GID( const GID& _o )
	: mID(_o.mID),
	  mHue(_o.mHue) {
	}

	inline bool GID::operator ==(const GID& _o ) const {
		return (mID==_o.mID&&
				mHue->isPartialHue()==_o.mHue->isPartialHue()&&
				mHue->getID()==_o.mHue->getID());
	}

	inline GID::operator QString() const {
		return QString("%1[%2]").arg(mID,0,16).arg(*mHue);
	}

	inline ID GID::getID() const {
		return mID;
	}

	inline Hue GID::getHue() const {
		return mHue;
	}

	inline GID& GID::setHue(Hue _newHue) {
		mHue = _newHue;
		return *this;
	}
}

#endif /* GID_HPP_ */
