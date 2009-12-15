/*
 * Animation.hpp
 *
 *  Created on: 13.12.2009
 *      Author: idstein
 */

#ifndef ANIMATION_HPP_
#define ANIMATION_HPP_

#include "IndexFile.hpp"
#include "Object.hpp"

namespace resource {
	class Animations: public IndexFile {
	public:
		typedef QPoint Center;
		typedef QPair<Center, Image> Frame;
		class Sequence {
			public:
				Sequence(Action _action, Direction _direction, QVector<Frame> _frames);
				Sequence(const Sequence& o);
				~Sequence();
				bool isValid() const;
				Action getAction() const;
				Direction getDirection() const;
				QVector<Frame> getFrames() const;
			private:
				Action mAction;
				Direction mDirection;
				QVector<Frame> mFrames;
		};
		typedef QHash<Direction, Sequence> Directions;
		typedef QHash<Action, Directions> Actions;
		class Entry: public Object {
			public:
				Entry(QWeakPointer<Animations> _loader, Body _body, QSharedPointer<
						Hues::Entry> _hue);
				virtual ~Entry();
				Entry& addSequence(const Sequence& _sequence);
				Sequence getSequence(Action _action, Direction _direction);
				Actions getActions() const;
				Body getBody() const;
				QSharedPointer<Hues::Entry> getHue() const;
			private:
				Body mBody;
				QSharedPointer<Entry> mHue;
				QWeakPointer<Animations> mLoader;
				Actions mActions;
		};
		enum Type {
			Monster, Animal, Character, Equipment
		};
		Animations(QString _indexFile, QString _dataFile,
				QObject* _parent);
		virtual ~Animations();
		Animations& loadBodyDef(QString _fileName);
		QSharedPointer<Entry> getAnimation(Body _body, Action _action,
				Direction _direction, QSharedPointer<Hues::Entry> _hue);
		Animations& loadSequence(Entry* _animation, Action _action, Direction _direction);
		private:
			//Animations& loadBodyConvDev(QString _fileName);
			//Animations& loadMobTypesTxt(QString _fileName);
			Animations& getFallback(Body& _body, Action& _action, Direction& _direction,
					QSharedPointer<Hues::Entry>& _hue);
			ID calculateID(Body _body, Action _action, Direction _direction );
			//IndexFile& getAnimFile(Body& _id);
			Sequence decode(QByteArray _data, Action _action, Direction _direction, QSharedPointer<
					Hues::Entry> _hue);
			Frame decodeFrame( QDataStream& _stream, Direction _direction, QVector<Colour> _hueTable );
			// QPair<Body, ID> - quint32 body, quint32 hue
			QMap<Body, QPair<Body, ID> > mFallback;
			// QPair<quint32, qint8> - quint32 body, qint8 file
			//QMap<quint32, QPair<quint32, qint8> >  mFileMapping;
			// QPair<quint32, quint32> - eAnimationType type, quint32 flags
			//QMap<quint32, QPair<Type, quint16> > mMobTypes;
			static const quint8 mDirectionMap[8];
			// describes amount of bodies and actions per section
			typedef QPair<Body,Action> SectionStructure;
			typedef QPair<Type, SectionStructure> Section;
			QVector<Section> mStructure;
	};

	inline bool Animations::Sequence::isValid() const {
		return (mFrames.size() > 0) ? true : false;
	}

	inline Action Animations::Sequence::getAction() const {
		return mAction;
	}

	inline Direction Animations::Sequence::getDirection() const {
		return mDirection;
	}

	inline Animations::Entry & Animations::Entry::addSequence(const Animations::Sequence & _sequence) {
		mActions[_sequence.getAction()][_sequence.getDirection()] = _sequence;
	}

	inline QVector<Animations::Frame> Animations::Sequence::getFrames() const {
		return mFrames;
	}

	inline Animations::Actions Animations::Entry::getActions() const {
		return Actions;
	}

	inline Body Animations::Entry::getBody() const {
		return mBody;
	}

	inline QSharedPointer<Hues::Entry> Animations::Entry::getHue() const {
		return mHue;
	}
}

#endif /* ANIMATION_HPP_ */
