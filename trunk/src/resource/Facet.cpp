#include "CMaps.h"
#include "CResourceHandler.h"
#include <Qt/QStringList.h>

using namespace resource;

CStaticItem::CStaticItem(SStaticItem *_item) 
: m_item(_item) {
}

CStaticItem::~CStaticItem() {
	if(m_item)
		delete m_item;
}

quint32 CStaticItem::id() const {
	return m_item->id;
}

qint16 CStaticItem::z() const {
	return m_item->z;
}

quint32 CStaticItem::hue() const {
	return m_item->color;
}

quint32 CStaticItem::xOffset() const {
	return m_item->xoffset;
}

quint32 CStaticItem::yOffset() const {
	return m_item->yoffset;
}

//IItemInformation& CStaticItem::info() {
//	return m_info;
//}

CMapItem::CMapItem(SMapCell *_item, quint32 _xOffset, quint32 _yOffset) 
: m_item(_item), m_xOffset(_xOffset), m_yOffset(_yOffset) {
}

CMapItem::~CMapItem() {
	if(m_item)
		delete m_item;
}

quint32 CMapItem::id() const {
	return m_item->id;
}

qint16 CMapItem::z() const {
	return m_item->z;
}

quint32 CMapItem::hue() const {
	return 0;
}

quint32 CMapItem::xOffset() const {
	return m_xOffset;
}

quint32 CMapItem::yOffset() const {
	return m_yOffset;
}

//IItemInformation& CMapItem::info() {
//	return m_info;
//}

CMapBlock::CMapBlock( quint8 _facet, quint32 _id, quint32 _x, quint32 _y, quint32 _w, quint32 _h ) :
CResource( CMaps::getKey(_facet, _id )), m_facet(_facet), m_id(_id), m_pos(QRect( _x, _y, _w, _h)) {
}

CMapBlock::~CMapBlock() {
	foreach(IItem* item, m_items) {
		if(item)
			delete item;
	}
	m_items.clear();
}

quint8 CMapBlock::facet() const {
	return m_facet;
}

quint32 CMapBlock::id() const {
	return m_id;
}


QRect CMapBlock::pos() const {
	return m_pos;
}

QVector<IItem*>& CMapBlock::items() {
	return m_items;
}



CMaps::CMaps( )
: m_active(NULL) {
	load();
}


CMaps::~CMaps() {
	unload();
}

SFacet* CMaps::active() {
	return m_active;
}

bool CMaps::setActiveFacet( SFacet* _facet ) {
	// Open files
	if (!_facet->map.isOpen()&&!_facet->map.open(QIODevice::ReadOnly)) {
		qWarning() << "Unable to open map file at" << m_map.fileName();
		return false;
	}
	if (!_facet->index.isOpen()&&!_facet->index.open(QIODevice::ReadOnly)) {
		qWarning() << "Unable to open the statics index at" << m_index.fileName();
		return false;
	}
	if (!_facet->index.isOpen()&&!_facet->statics.open(QIODevice::ReadOnly)) {
		qWarning() << "Unable to open statics data at" << m_data.fileName();
		return false;
	}
	m_mapStream.setDevice(&(_facet->map));
	m_dataStream.setDevice(&(_facet->statics));
	m_indexStream.setDevice(&(_facet->index));
	SFacet* _active = active();
	if(_active) {
		_active->map.close();
		_active->index.close();
		_active->statics.close();
	}
	m_active = _facet;
	return true;
}

bool CMaps::load() {
	QSettings* settings = Resources::instance().settings();
	QString path = settings->value( "path", "" ).toString();
	settings->beginGroup("Maps");
		quint8 i = 0;
		foreach( QString subfile, settings->childGroups() ) {
			if( !subfile.isEmpty()) {
				settings->beginGroup(subfile);
				QString dataFile = settings->value("data", "").toString();
				QString indexFile = settings->value("index", "").toString();
				QString mapFile = settings->value("map", "").toString();
				quint32 width = settings->value("width", 0).toUInt();
				quint32 height = settings->value("height", 0).toUInt();
				if (!dataFile.isEmpty() && !indexFile.isEmpty() && !mapFile.isEmpty()) {
					qDebug() << "Adding map facet" << dataFile << indexFile << mapFile;
					SFacet* facet = new SFacet;
					facet->id = i;
					facet->width = width;
					facet->height = height;
					Q_CHECK_PTR(facet);

					facet->statics.setFileName( path + dataFile );
					facet->index.setFileName( path + indexFile );
					facet->map.setFileName( path + mapFile );
					if (!facet->statics.open(QIODevice::ReadOnly)||
						!facet->index.open(QIODevice::ReadOnly)||
						!facet->map.open(QIODevice::ReadOnly)) {
						qCritical() << "Couldn't create facet " << facet->statics.fileName() << facet->index.fileName() << facet->map.fileName();
						delete facet;
					} else {
						m_maps.append( facet );
					};
				}
				i++;
				settings->endGroup();
			}
		}
	settings->endGroup();
	if( !m_maps.isEmpty() && setActiveFacet( m_maps.front() ) ) {
		m_indexStream.setDevice(&(m_maps.front()->index));
		m_dataStream.setDevice(&(m_maps.front()->statics));
		m_dataStream.setDevice(&(m_maps.front()->map));
		return true;
	} else {
		qCritical() << "No map files found!";
		return false;
	}
}

bool CMaps::unload() {
	foreach(SFacet* facet, m_maps) {
		facet->map.close();
		facet->index.close();
		facet->statics.close();
		delete facet;
	}
	return true;
}

bool CMaps::reload() {
	return ( unload() && load() );
}

bool CMaps::switchFacet(quint8 _id) {
	if( _id > m_maps.size())
		return false;
	// do not switch, if already active facet
	if( m_maps[_id] = m_active )
		return true;
	SFacet* _active = active();
	if( !setActiveFacet( m_maps[_id] )) {
		setActiveFacet(_active);
		return false;
	} else {
		return true;
	}
}

bool CMaps::switchFacet(QString _name) {
	for( qint8 i = 0; i < m_maps.size(); i++ ) {
		if(m_maps[i]->name == _name) {
			return switchFacet( i );
		}
	}
	return false;
}

CMapBlock* CMaps::getMapBlock(quint32 _x, quint32 _y) {
	CMapBlock* result = NULL;
	if( _x < active()->width && _y < active()->height ) {
		quint32 blockid_ = (_x/8)*(active()->height/8) + (_y/8);
		// check 2nd lvl cache
		result = reinterpret_cast<CMapBlock*>(Resources::instance().cache()->available( getKey( active()->id, blockid_ ) ) );
		if(result)
			return result;
		// load from active facet
		result = new CMapBlock( active()->id, blockid_, (_x/8), (_y/8) );
		Q_CHECK_PTR(result);
		// load map block from mapX.mul
		m_mapStream.device()->seek( 196 * blockid_ + 4 );
		QVector<IItem*>& resItems = result->items();
		SMapCell* cells = new SMapCell[64];
		Q_CHECK_PTR(cells);
		m_mapStream.readRawData( (char*)cells, sizeof(SMapCell)*64 );
		for(quint8 i = 0; i < 64; i ++ )
			resItems.append( new CMapItem( &cells[i], i%8,i/8 ) ); 
		
		// seek to appropriate statics offset
		m_indexStream.device()->seek( blockid_ *12 );
		quint32 offset_ = 0xFFFFFFFF;
		quint32 length_ = 0xFFFFFFFF;
		m_indexStream >> offset_ >> length_;
		if( offset_ != 0xFFFFFFFF ) {
			m_dataStream.device()->seek(offset_);
			quint32 numStatics_ = length_/7;
			SStaticItem* items = new SStaticItem[numStatics_];
			Q_CHECK_PTR(items);
			m_dataStream.readRawData( (char*)items, sizeof(SStaticItem) * numStatics_ );
			for(quint32 i = 0; i < numStatics_; i ++ )
				resItems.append( new CStaticItem( &items[i] ) ); 
		}
		return result;
	} else {
		result = reinterpret_cast<CMapBlock*>(Resources::instance().cache()->available( getKey( 0xFF, 0xFFFFFFFF ) ) );
		if(!result)
			result = new CMapBlock( 0xFF, 0xFFFFFFFF, 0,0 );
		return result;
	}
}

CMapBlock* CMaps::getMapBlock( quint32 _id ) {
	CMapBlock* result = NULL;
	// check 2nd lvl cache
	result = reinterpret_cast<CMapBlock*>(Resources::instance().cache()->available( getKey( active()->id, _id ) ) );
	if(result)
		return result;
	// load from active facet
	result = new CMapBlock( active()->id, _id, _id/active()->height, _id%active()->height );
	Q_CHECK_PTR(result);
	// load map block from mapX.mul
	m_mapStream.device()->seek( 196 * _id + 4 );
	QVector<IItem*>& resItems = result->items();
	SMapCell* cells = new SMapCell[64];
	Q_CHECK_PTR(cells);
	m_mapStream.readRawData( (char*)cells, sizeof(SMapCell)*64 );
	for(quint8 i = 0; i < 64; i ++ )
		resItems.append( new CMapItem( &cells[i], i%8,i/8 ) ); 
	
	// seek to appropriate statics offset
	m_indexStream.device()->seek( _id *12 );
	quint32 offset_ = 0xFFFFFFFF;
	quint32 length_ = 0xFFFFFFFF;
	m_indexStream >> offset_ >> length_;
	if( offset_ != 0xFFFFFFFF ) {
		m_dataStream.device()->seek(offset_);
		quint32 numStatics_ = length_/7;
		SStaticItem* items = new SStaticItem[numStatics_];
		Q_CHECK_PTR(items);
		m_dataStream.readRawData( (char*)items, sizeof(SStaticItem) * numStatics_ );
		for(quint32 i = 0; i < numStatics_; i ++ )
			resItems.append( new CStaticItem( &items[i] ) ); 
	}
	return result;
}

QString CMaps::getKey(quint8 _facet, quint32 _id) {
	return QString("Map%1i%2").arg(_facet).arg(_id);
}