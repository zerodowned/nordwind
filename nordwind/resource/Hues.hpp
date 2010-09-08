#ifndef CHUES_H
#define CHUES_H

#include <qobject.h>
#include <qvector.h>
#include <qdatastream.h>
#include <qsharedpointer.h>
#include "util/Utilities.hpp"

namespace resource {
	class Hues : public QObject {
		public:
			struct RawHue {
				ID mID;
				QString mName;
				Colour mStartColour;
				Colour mEndColour;
				QVector<Colour> mColourTable;
			};
			class Entry {
				public:
					Entry(QSharedPointer<RawHue> _data);
					virtual ~Entry();
					ID getID() const;
					QVector<Colour> getColourTable() const;
					Colour getStartColour() const;
					Colour getEndColour() const;
					QString getName() const;
					bool isPartialHue() const;
					virtual Colour mapToHue(Colour16 _colour16) const;
					operator QString() const;
				protected:
					QSharedPointer<RawHue> mRawHue;
			};
			class PartialHue: public Entry {
				public:
					PartialHue(QSharedPointer<RawHue> _data);
					virtual Colour mapToHue(Colour16 _colour16) const;
			};
			class DefaultHue: public Entry {
				public:
					DefaultHue();
					virtual Colour mapToHue(Colour16 _colour16) const;
			};
			Hues(QString _dataFile, QObject* _parent);
			virtual ~Hues();
			QVector<QSharedPointer<RawHue> > getHues() const;
			QSharedPointer<Entry> getHue( ID _id, bool _partialHue ) const;
			QSharedPointer<Entry> getDefaultHue() const;
		protected:
			Hues& load();
		private:
			QSharedPointer<DefaultHue> mDefaultHue;
			QVector< QSharedPointer<RawHue> > mHues;
			QDataStream mDataStream;
	};

	typedef QSharedPointer<Hues::Entry> Hue;

	inline Hues::Entry::Entry(QSharedPointer<Hues::RawHue> _data )
	: mRawHue(_data) {
	}

	inline Hues::Entry::~Entry() {
	}

	inline ID Hues::Entry::getID() const {
		return mRawHue->mID;
	}

	inline QVector<Colour> Hues::Entry::getColourTable() const {
		return mRawHue->mColourTable;
	}

	inline Colour Hues::Entry::getStartColour() const {
		return mRawHue->mStartColour;
	}

	inline Colour Hues::Entry::getEndColour() const {
		return mRawHue->mEndColour;
	}

	inline QString Hues::Entry::getName() const {
		return mRawHue->mName;
	}

	inline bool Hues::Entry::isPartialHue() const {
		return (dynamic_cast<const Hues::PartialHue*>(this)!=NULL) ? true : false;
	}

	inline Hues::Entry::operator QString() const {
		return QString("%1(%2)").arg(mRawHue->mName).arg(mRawHue->mID,0,16);
	}

	inline Hues::PartialHue::PartialHue( QSharedPointer<Hues::RawHue> _data )
	: Hues::Entry(_data) {
	}

	inline Hues::DefaultHue::DefaultHue()
	: Hues::Entry( QSharedPointer<Hues::RawHue>( new Hues::RawHue ) ) {
		mRawHue->mID = 0;
	}

	inline QVector<QSharedPointer<Hues::RawHue> > Hues::getHues() const {
		return mHues;
	}

	inline QSharedPointer<Hues::Entry> Hues::getHue(ID _id, bool _partialHue) const {
                if (_id<=1 || ((int)(_id) > mHues.size())) {
			return getDefaultHue();
		} else {
			if(_partialHue) {
                                return QSharedPointer<Hues::PartialHue>( new Hues::PartialHue(mHues[_id-2]) );
			} else {
                                return QSharedPointer<Entry>( new Hues::Entry(mHues[_id-2]) );
			}
		}
	}

	inline QSharedPointer<Hues::Entry> Hues::getDefaultHue() const {
            return mDefaultHue.staticCast<Hues::Entry>();
	}
}
#endif
