#if !defined(__ART_H__)
#define __ART_H__

#include "IndexFile.hpp"
#include "Texture.hpp"

namespace resource {

	class Arts : public IndexFile {
		public:
			class Entry : protected Texture {
				public:
					Entry( ID _id, Image _image, QSharedPointer<Hues::Entry> _hue );
					virtual ~Entry();
			};
			struct Animation {
				QVector<ID> mIDs;
				quint8 mFrameDelay;
				quint8 mStartDelay;
			};
			class AnimationEntry : protected Entry {
				public:
					AnimationEntry( ID _id, Image _image, QSharedPointer<Hues::Entry> _hue, QVector< QSharedPointer<Entry> > _frames, QSharedPointer<Animation> _animData );
					virtual ~AnimationEntry();
					QSharedPointer<Animation> getAnimData() const;
				protected:
					QSharedPointer<Animation> mAnimData;
					QVector< QSharedPointer<Entry> > mFrames;
			};
			Arts( QString _animFile, QString _indexFile, QString _dataFile, QObject* _parent );
			virtual ~Arts();
			QSharedPointer<Entry> getArtStatic( ID _id, QSharedPointer<Hues::Entry> _hue );
			QSharedPointer<Entry> getArtLand( ID _id, QSharedPointer<Hues::Entry> _hue );
			QSharedPointer<Entry> getEntry( ID _id, QSharedPointer<Hues::Entry> _hue );
		protected:
			QSharedPointer<Animation> getAnimData( ID _id );
			Image decodeStatic( QByteArray _data, QSharedPointer<Hues::Entry> _hue );
			Image decodeLand( QByteArray _data, QSharedPointer<Hues::Entry> _hue );
			QDataStream mAnimStream;
			QMap<ID, qint64> mAnimDataOffsets;
	};

	inline Arts::Entry::Entry( ID _id, Image _image, QSharedPointer<Hues::Entry> _hue )
	: Texture(Object::Arts, _id, _image, _hue) {
	}

	inline Arts::Entry::~Entry() {
	}

	inline Arts::AnimationEntry::AnimationEntry( ID _id, Image _image, QSharedPointer<Hues::Entry> _hue, QVector< QSharedPointer<Entry> > _frames, QSharedPointer<Animation> _animData )
	: Arts::Entry( _id, _image, _hue ), mAnimData(_animData), mFrames(_frames) {
		QSharedPointer<Entry> cur(this);
		foreach( QSharedPointer<Entry> frame, mFrames ) {
			if(frame.isNull()) {
				frame = cur;
			}
		}
	}

	inline Arts::AnimationEntry::~AnimationEntry() {
	}

	inline QSharedPointer<Arts::Animation> Arts::AnimationEntry::getAnimData() const {
		return mAnimData;
	}
			// Read a cursor and fill the given cursor structure with the corresponding data
			QImage* getCursor(qint32& _hotX, qint32& _hotY, quint32 _id, quint32 _hue = 0, bool _partialhue = false);
}

#endif
