/*
 * Mobile.cpp
 *
 *  Created on: 25.12.2009
 *      Author: idstein
 */
#include "Mobile.hpp"
#include <qpainter.h>

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
    if (mID < 200)
        result = mID * 110;
    else if (mID < 400)
        result = (mID - 200) * 65 + 22000;
    else
        result = (mID - 400) * 175 + 35000;
    result += mAction * 5 + sDirectionMap[mDirection & 7];
    return result;
}

ID Mobile::body() const {
    return mID;
}

Mobile::Action Mobile::action() const {
    return mAction;
}

Mobile::Direction Mobile::direction() const {
    return mDirection;
}

QRectF Mobile::boundingRect() const {
    return mBase.frameRect();
}

Mobile::Mobile(const Serial &serial, QObject *parent, const ID &body, const Direction &direction, Action action)
    : Dynamic(serial,parent),
    Object(body),
    QGraphicsPixmapItem(),
    mAction(action),
    mDirection(direction),
    mBase(QString("animations:%1").arg(id()),"anim",this) {
    if(!mBase.isValid())
        qDebug() << "Invalid animation";
    mBase.setCacheMode(QMovie::CacheAll);
    connect(&mBase,SIGNAL(frameChanged(int)),SLOT(onFrameChanged(int)));
    mBase.start();
}

/*void Mobile::paint( QPainter* painter, const QStyleOptionGraphicsItem* _option, QWidget* _widget ) {
    Q_UNUSED(_option);
    Q_UNUSED(_widget);
    QRect rect = mBase.frameRect();
    if(sDirectionFlip[mDirection])
        painter->setTransform(QTransform(-1.0f,0.0f,
                                         0.0f,1.0f,
                                         rect.width(),0.0f));
    painter->drawPixmap(rect.topLeft(),mBase.currentPixmap());
}*/

void Mobile::onFrameChanged(int frameNumber) {
    Q_UNUSED(frameNumber);
    setPixmap(mBase.currentPixmap());
    setOffset(mBase.frameRect().topLeft());
    //update();
}
