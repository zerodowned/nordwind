/*
 * Mobile.hpp
 *
 *  Created on: 22.12.2009
 *      Author: idstein
 */

#ifndef MOBILE_HPP_
#define MOBILE_HPP_

#include "Dynamic.hpp"
#include "../core/Resources.hpp"
#include <qparallelanimationgroup.h>

class QStateMachine;

namespace game {
	class Mobile : public Dynamic {
		Q_OBJECT;
		public:
			// TODO create a subclass for running animations here
			Mobile( const Coordinate& _position, Body _body, ID _hueID );
			Body getBody() const;
			Mobile& setBody( Body _newBody );
			Action getAction() const;
			Mobile& setAction( Action _action );
			Direction getDirection() const;
			Mobile& setDirection( Direction _direction );
//			ID getHue() const;
//			Mobile& setHue() const;
			void paint( QPainter* _painter, const QStyleOptionGraphicsItem* _option, QWidget* _widget = 0 );
		Q_SIGNALS:
			void bodyChange(Body _newBody,ID _newHue,bool _partialHue);
			void animationChange(Action _action,Direction _direction);
		public Q_SLOTS:
			void onFrameChanged( quint8 _nextFrame );
			void onAnimationChanged( Action _action, Direction _direction );
			void onBodyChanged( Body _newBody, ID _newHue, bool _partialHue );
		private:
			QStateMachine* mInternalState;
			Action mAction;
			Direction mDirection;
			quint8 mCurrentFrame;
			resource::Animation mData;
	};

	inline Body Mobile::getBody() const {
		if(!mData.isNull())
			return mData->getBody();
		return -1;
	}

	inline Action Mobile::getAction() const {
		return mAction;
	}

	inline Mobile& Mobile::setAction(Action _action) {
		emit animationChange(_action,mDirection);
		return *this;
	}

	inline Direction Mobile::getDirection() const {
		return mDirection;
	}

	inline Mobile& Mobile::setDirection(Direction _direction) {
		emit animationChange(mAction,_direction);
		return *this;
	}
}


#endif /* MOBILE_HPP_ */
