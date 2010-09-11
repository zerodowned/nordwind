/*
 * Mobile.cpp
 *
 *  Created on: 25.12.2009
 *      Author: idstein
 */
#include "Mobile.hpp"
#include <qpainter.h>
#include "../Client.hpp"

using namespace game;

// West = 0, Right = 1, East = 2, Down = 3, South = 4, Left = 5, North = 6, Up = 7
const quint8 Mobile::sDirectionMap[8] = { 3, 2, 1, 0, 1, 2, 3, 4 };
// we need to mirror the output texture on West, Right and East
const bool Mobile::sDirectionFlip[8] = { true, true, true, false, false, false,
		false, false };

//IndexFile& Animations::getAnimFile(Body& _body) const {
//	if(!mFileMapping.contains( _body ))
//		return _body;
//	qint8 file = mFileMapping[_body].second;
//	if(!mAnimFiles.contains(file))
//		return *this;
//	_body = mFileMapping[_body].first;
//	return file;
//}

//Animations& Animations::loadBodyDef( QString _fileName ) {
//	// Format is:
//	// oldbody {newbody, newbody, newbody} newhue
//	QRegExp pattern("^\\s*(\\d+)\\s+\\{([^\\}]+)\\}\\s+(\\d+)\\s*$");
//	QRegExp splitPattern("\\s*,\\s*");
//
//	QFile file(_fileName);
//	QTextStream stream(&file);
//	Q_ASSERT_X(stream.device()->open(QIODevice::ReadOnly), __PRETTY_FUNCTION__, _fileName.toAscii().constData());
//	bool ok = false;
//	quint32 iLine = 0;
//	while (!stream.atEnd()) {
//		QString line = stream.readLine();
//		++iLine;
//		if (!line.startsWith("//") && line.contains(pattern)) {
//			// Parse the list of new ids
//			QStringList newIds = pattern.cap(2).split(splitPattern);
//			Body oldBody = pattern.cap(1).toUInt(&ok);
//			if(!ok)
//				oldBody = pattern.cap(1).toUInt( &ok, 16 );
//			if(!ok) {
//				qDebug() << "Incorrect body in" << file.fileName() << " at line" << iLine;
//				continue;
//			}
//			ID hue = pattern.cap(3).toUInt();
//			if (newIds.size() > 0) {
//				mFallback.insert(oldBody, QPair<Body, ID>(  newIds[0].toUInt(), hue ));
//			}
//		}
//	}
//	return *this;
//}

//Animations& Animations::loadMobTypesTxt(QString path) {
//	QFile file(path);
//
//	if (!file.open(QFile::ReadOnly)) {
//		qDebug() << "Unable to open mobile type data from " << file.fileName();
//		return;
//	}
//
//	// Format is:
//	// body type(string) flags
//	QTextStream stream(&file);
//	quint32 iLine = 0;
//	while (!stream.atEnd()) {
//		QString line = stream.readLine().trimmed();
//		++iLine;
//		if (!line.startsWith("//")) {
//			QStringList parts = line.split(QRegExp("\\s+"));
//
//			// 3 parts required
//			if (parts.size() == 3) {
//				bool ok = false;
//				ushort body = parts[0].toUInt( &ok );
//				if(!ok) {
//					body = parts[0].toUInt( &ok, 16 );
//				}
//				if(!ok) {
//					qDebug() << "Incorrect body in" << file.fileName() << " at line" << iLine;
//					continue;
//				}
//
//				quint16 flags = parts[2].toUShort(&ok); // Parse flags
//				if(!ok) {
//					qDebug() << "Incorrect flag in " << file.fileName() << " at line" << iLine;
//					continue;
//				}
//
//				// Parse the body type
//                eAnimationType type;
//				if (parts[1] == "HUMAN") {
//					type = eAnim_human;
//				} else if (parts[1] == "ANIMAL") {
//					type = eAnim_animal;
//				} else if (parts[1] == "MONSTER") {
//					type = eAnim_monster;
//				} else if (parts[1] == "EQUIPMENT") {
//					type = eAnim_equipment;
//				} else {
//					qDebug() << "Unknown type in" << file.fileName() << " at line" << iLine;
//					continue;
//				}
//				m_mobTypes.insert( (quint32)body, QPair<eAnimationType, quint16>(type, flags) );
//			}
//		}
//	}
//}

//Animations& Animations::getFallback( Body& _body, Action& _action, Direction& _direction, Hue&_hue ) {
//	Q_UNUSED(_action);
//	Q_UNUSED(_direction);
//	if(mFallback.contains(_body)) {
//		_hue = Client::getInstance()->resources()->hues()->getHue( mFallback[_body].second, _hue->isPartialHue() );
//		_body = mFallback[_body].first;
//	}
//	return *this;
//}

//	Anim2
//		200 monsters � 110 actions
//		x animals � 65 actions
//	Anim3
//		300 animals � 65 actions
//		100 monsters � 110 actions
//		x equipement � 175 actions
//	Anim4
//		200 animals � 65 actions
//		200 monsters � 110 actions
//		x equipement ``a 175 actions
ID Mobile::id() const {
	ID result = 0;
	if (mBody < 200)
		result = mBody * 110;
	else if (mBody < 400)
		result = (mBody - 200) * 65 + 22000;
	else
		result = (mBody - 400) * 175 + 35000;
	result += mAction * 5 + sDirectionMap[mDirection & 7];
	return result;
}

Mobile::Mobile(const QPoint& p, Z _z, Mobile::Body body, Hue hue, Mobile::Action action, Mobile::Direction direction )
: Dynamic(Coordinate(p,_z)),
mBody(body),
mAction(action),
mDirection(direction),
mHue(hue),
mBase(Client::getInstance()->resources().animationFile() + ":" + QString::number(id()),"animation",this) {
	mBase.setCacheMode(QMovie::CacheAll);
	connect(&mBase,SIGNAL(frameChanged(int)),SLOT(onFrameChanged(int)));
	mBase.start();
}

void Mobile::onFrameChanged(int frameNumber) {
	Q_UNUSED(frameNumber);
	QRect rect = mBase.frameRect();
	setOffset(rect.topLeft()-QPoint(rect.width()-44,rect.height()-44));
	QPixmap pixmap = sDirectionFlip[mDirection]?QPixmap::fromImage(mBase.currentImage().mirrored()):mBase.currentPixmap();
	setPixmap(pixmap);
}
