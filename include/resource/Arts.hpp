#if !defined(__ART_H__)
#define __ART_H__

#include "IndexFile.hpp"
#include "Hues.hpp"
#include <qmap.h>

namespace resource {

	class Arts : public IndexFile {
		public:
			struct Animation {
				QVector<ID> mIDs;
				quint8 mFrameDelay;
				quint8 mStartDelay;
			};
			Arts( QString _animFile, QString _indexFile, QString _dataFile, QObject* _parent );
			QPixmap getArtStatic( ID _id, Hue _hue );
			QPixmap getArtLand( ID _id, Hue _hue );
			QPixmap getEntry( ID _id, Hue _hue );
			Animation getAnimData( ID _id );
		protected:
			QPixmap decodeStatic( QByteArray _data, Hue _hue );
			QPixmap decodeLand( QByteArray _data, Hue _hue );
			QDataStream mAnimStream;
			QMap<ID, qint64> mAnimDataOffsets;
	};

	inline QPixmap Arts::getArtStatic( ID _id, Hue _hue ) {
	    ID id = _id + 0x4000;
		return getEntry(id,_hue);
	}

	inline QPixmap Arts::getArtLand( ID _id, Hue _hue ) {
		return getEntry(_id,_hue);
	}

	inline QPixmap Arts::getEntry( ID _id, Hue _hue ) {
		QPixmap result;
		QByteArray data = getData(_id);
		if(!data.isEmpty()) {
			result = (_id<0x4000) ? decodeLand(data,_hue) : decodeStatic(data,_hue);
		}
		return result;
	}
}

#endif
