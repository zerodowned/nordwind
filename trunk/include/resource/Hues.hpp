#ifndef CHUES_H
#define CHUES_H

#include <qobject.h>
#include <qvector.h>
#include <qdatastream.h>
#include "../../util/Utilities.hpp"

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
				protected:
					QSharedPointer<RawHue> mRawHue;
			};
			class PartialHue: protected Entry {
				public:
					PartialHue(QSharedPointer<RawHue> _data);
					virtual Colour mapToHue(Colour16 _colour16) const;
			};
			class DefaultHue: protected Entry {
				public:
					DefaultHue(QSharedPointer<RawHue> _data);
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
		return (dynamic_cast<Hues::PartialHue*>(this)!=NULL) ? true : false;
	}

	inline Colour Hues::Entry::mapToHue( Colour16 _colour ) const {
		return mRawHue->mColourTable[qRed(_colour)>>3];
	}

	inline Hues::PartialHue::PartialHue( QSharedPointer<Hues::RawHue> _data )
	: Hues::Entry(_data) {
	}

	inline Colour Hues::PartialHue::mapToHue( Colour16 _colour ) const {
		quint8 r = qRed(_colour);
		if(r==qBlue(_colour)&&
		   r==qGreen(_colour)) {
			return mRawHue->mColourTable[r>>3];
		} else {
			return qRgb(_colour);
		}
	}

	inline Hues::DefaultHue::DefaultHue( QSharedPointer<Hues::RawHue> _data )
	: Hues::Entry(_data) {
	}

	inline Colour Hues::DefaultHue::mapToHue( Colour16 _colour ) const {
		return qRgb(_colour);
	}

	inline QVector<QSharedPointer<RawHue> > Hues::getHues() const {
		return mHues;
	}

	inline QSharedPointer<Hues::Entry> Hues::getHue(ID _id, bool _partialHue) const {
		if (_id > mHues.size())
			return mDefaultHue;
		return QSharedPointer<Entry>( (_partialHue) ? new Hues::PartialHue(mHues[_id])
												  : new Hues::Entry(mHues[_id]) );
	}

	inline QSharedPointer<Hues::Entry> Hues::getDefaultHue() const {
		return mDefaultHue;
	}
}
#endif
