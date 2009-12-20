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
			operator QString() const;
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
		return QString("ID%1HUE%2").arg(mID).arg(mHue->getID());
	}
}

#endif /* GID_HPP_ */
