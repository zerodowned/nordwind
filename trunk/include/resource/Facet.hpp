#ifndef CMAPS_H_
#define CMAPS_H_

#include <Qt/QFile.h>
#include <Qt/QDataStream.h>
#include <Qt/QVector.h>
#include <Qt/QRect.h>

namespace resource {

	class Facet {
		QFile m_map;
		QFile m_statics;
		QFile m_index;
		QString m_name; // optional
		quint32 m_width; // width in cells
		quint8 m_cellsX; // block in cellsX
		quint32 m_height; // height in cells
		quint8 m_cellsY; // block in cellsY
		quint8 m_id;
	public:
		// interface
		quint8 id() const;
		quint32 width() const;
		quint32 height() const;
		QString name() const;
		quint32 blockId( quint32 _cellX, quint32 _cellY );
	};

	class CMaps {
	protected:
			QVector<SFacet*> m_maps;
			QFile m_map, m_data, m_index;
			QDataStream m_mapStream, m_dataStream, m_indexStream;
			SFacet* m_active;
			bool setActiveFacet( SFacet* _facet );
	public:
		CMaps( );
		virtual ~CMaps();
		bool load();
		bool unload();
		bool reload();

		SFacet* active();
		bool switchFacet( quint8 _id );
		bool switchFacet( QString _name );
		CMapBlock* getMapBlock( quint32 _id );
		CMapBlock* getMapBlock( quint32 _x, quint32 _y );
		static QString key( quint8 _facet, quint32 _id  );
	};
}
#endif
