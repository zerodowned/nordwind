/*
 * Mobile.cpp
 *
 *  Created on: 25.12.2009
 *      Author: idstein
 */
#include "game/Mobile.hpp"
#include <qstatemachine.h>
#include <qpainter.h>
#include "Client.hpp"

using namespace game;

Mobile::Mobile(const Coordinate& _position, Body _body, ID _hueID)
: Dynamic(_position),
  mInternalState(new QStateMachine(this)),
  mAction(0x4),
  mDirection(Down),
  mCurrentFrame(0),
  mData(Client::getInstance()->resources()->getAnimation(_body,_hueID)){
	onAnimationChanged(mAction,mDirection);
}

void Mobile::paint(QPainter* _painter,const QStyleOptionGraphicsItem* _option,QWidget* _widget) {
	if(mData.isNull())
		return;
	_painter->drawPixmap(mData->getSequence(mAction,mDirection)[mCurrentFrame].first,
						 mData->getSequence(mAction,mDirection)[mCurrentFrame].second);
}

void Mobile::onBodyChanged(Body _newBody,ID _newHue,bool _partialHue) {
	mData = Client::getInstance()->resources()->getAnimation(_newBody,_newHue,_partialHue);
	mAction = 0x4; // STAND
	update();
}

void Mobile::onAnimationChanged(Action _action,Direction _direction) {
	if(mData.isNull())
		return;
	mAction = _action;
	mDirection = _direction;
	// TODO start Animation here
	update();
}

void Mobile::onFrameChanged(quint8 _newFrame) {
	mCurrentFrame = _newFrame;
	update();
}

Mobile& Mobile::setBody(Body _newBody) {
	if(mData.isNull()) {
		emit bodyChange(_newBody,0,false);
	} else {
		emit bodyChange(_newBody, mData->getHue()->getID(), mData->getHue()->isPartialHue() );
	}
	return *this;
}
