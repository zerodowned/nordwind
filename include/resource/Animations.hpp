/*
 * Animation.hpp
 *
 *  Created on: 13.12.2009
 *      Author: idstein
 */

#ifndef ANIMATION_HPP_
#define ANIMATION_HPP_

#include "IndexFile.hpp"
#include "Hues.hpp"
#include <qpair.h>
#include <qmap.h>

namespace resource {

	class Animation;
	class Animations: public IndexFile {
	public:
		enum Type {
			Monster, Animal, Character, Equipment
		};
		Animations(QString _indexFile, QString _dataFile,
				QObject* _parent);
		Animations& loadBodyDef(QString _fileName);
		Animations& getSequence(Animation* _animation, Action _action, Direction _direction);
		private:
			//Animations& loadBodyConvDev(QString _fileName);
			//Animations& loadMobTypesTxt(QString _fileName);
			Animations& getFallback(Body& _body, Action& _action, Direction& _direction,
					Hue& _hue);
			ID calculateID(Body _body, Action _action, Direction _direction );
			//IndexFile& getAnimFile(Body& _id);
			Sequence decode(QByteArray _data, Action _action, Direction _direction, Hue _hue);
			Frame decodeFrame( QDataStream& _stream, Direction _direction, QVector<Colour> _hueTable );
			// QPair<Body, ID> - quint32 body, quint32 hue
			QMap<Body, QPair<Body, ID> > mFallback;
			// QPair<quint32, qint8> - quint32 body, qint8 file
			//QMap<quint32, QPair<quint32, qint8> >  mFileMapping;
			// QPair<quint32, quint32> - eAnimationType type, quint32 flags
			//QMap<quint32, QPair<Type, quint16> > mMobTypes;
			static const quint8 mDirectionMap[8];
			static const bool mDirectionFlip[8];
			// describes amount of bodies and actions per section
			typedef QPair<Body,Action> SectionStructure;
			typedef QPair<Type, SectionStructure> Section;
			QVector<Section> mStructure;
	};
}

#endif /* ANIMATION_HPP_ */
