#if !defined(__ART_H__)
#define __ART_H__

#include "IndexFile.hpp"
#include "GraphicsTexture.hpp"
#include <qmap.h>

namespace resource {

	class Arts : public IndexFile {
		public:
			class Entry : public GraphicsTexture {
				public:
					Entry( ID _id, Image _image, Hue _hue );
					virtual ~Entry();
					bool isAnimated() const;
			};
			struct Animation {
				QVector<ID> mIDs;
				quint8 mFrameDelay;
				quint8 mStartDelay;
			};
			class AnimationEntry : public Entry {
				public:
					AnimationEntry( ID _id, Image _image, Hue _hue, QVector< QSharedPointer<Entry> > _frames, QSharedPointer<Animation> _animData );
					virtual ~AnimationEntry();
					QSharedPointer<Animation> getAnimData() const;
				protected:
					QSharedPointer<Animation> mAnimData;
					QVector< QSharedPointer<Entry> > mFrames;
			};
			Arts( QString _animFile, QString _indexFile, QString _dataFile, QObject* _parent );
			virtual ~Arts();
			QSharedPointer<Entry> getArtStatic( ID _id, Hue _hue );
			QSharedPointer<Entry> getArtLand( ID _id, Hue _hue );
			QSharedPointer<Entry> getEntry( ID _id, Hue _hue );
		protected:
			QSharedPointer<Arts::Animation> getAnimData( ID _id );
			Image decodeStatic( QByteArray _data, Hue _hue );
			Image decodeLand( QByteArray _data, Hue _hue );
			QDataStream mAnimStream;
			QMap<ID, qint64> mAnimDataOffsets;
	};

	typedef QSharedPointer<Arts::Entry> Art;

	inline Arts::Entry::Entry( ID _id, Image _image, Hue _hue )
	: GraphicsTexture(Object::Arts, _id, _image, _hue) {
	}

	inline Arts::Entry::~Entry() {
	}

	inline bool Arts::Entry::isAnimated() const {
		return (dynamic_cast<const Arts::AnimationEntry*>(this)!=NULL) ? true : false;
	}

	inline Arts::AnimationEntry::AnimationEntry( ID _id, Image _image, Hue _hue, QVector< QSharedPointer<Entry> > _frames, QSharedPointer<Animation> _animData )
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
//			QImage* getCursor(qint32& _hotX, qint32& _hotY, quint32 _id, quint32 _hue = 0, bool _partialhue = false);
}

#endif
