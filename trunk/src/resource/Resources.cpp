#include "CResourceHandler.h"
#include "CResourceCache.h"
#include "CHues.h"
#include "CAnimations.h"
#include "CArts.h"
#include "CGumps.h"
#include "CTextures.h"
#include "CMaps.h"
#include "plugindef.h"

using namespace resource;

Resources* Resources::m_instance = NULL;

Resources::Resources( QObject* _parent ) 
: IResource(_parent), m_animations(NULL),
m_arts(NULL),
m_gumps(NULL),
m_hues(NULL),
m_maps(NULL),
m_cache(NULL),
m_textures(NULL),
m_instance(this) {
}


Resources::~Resources() {
	unload();
	m_instance = NULL;
}

const QString& Resources::version() {
	return resourceVersion;
}

const QString& Resources::name() {
	return resourceName;
}

bool Resources::load() {
	if(!IModule::load())
		return false;

	qDebug() << "Init cache";
	m_cache = new CResourceCache();
	Q_CHECK_PTR(m_cache);

	qDebug() << "Init hues";
	m_hues = new Hues();
	Q_CHECK_PTR(m_hues);

	qDebug() << "Init animations";
	m_animations = new CAnimations();
	Q_CHECK_PTR(m_animations);

	qDebug() << "Init arts";
	m_arts = new CArts();
	Q_CHECK_PTR(m_arts);

	qDebug() << "Init gumps";
	m_gumps = new Gumps();
	Q_CHECK_PTR(m_gumps);

	qDebug() << "Init map";
	m_maps = new CMaps();
	Q_CHECK_PTR(m_maps);

	qDebug() << "Init textures";
	m_textures = new CTextures();
	Q_CHECK_PTR(m_textures);
	return true;
}

bool Resources::unload() {
	if(!IModule::unload())
		return false;
	if(m_animations) {
		delete m_animations;
		m_animations = NULL;
	}
	if(m_arts) {
		delete m_arts;
		m_arts = NULL;
	}
	if(m_gumps) {
		delete m_gumps;
		m_gumps = NULL;
	}
	if(m_hues) {
		delete m_hues;
		m_hues = NULL;
	}
	if(m_maps) {
		delete m_maps;
		m_maps = NULL;
	}
	if(m_cache) {
		delete m_cache;
		m_cache = NULL;
	}
	if(m_textures) {
		delete m_textures;
		m_textures = NULL;
	}
	return true;
}

qint32 Resources::status() {
	return 0;
}

QString Resources::statusDesc(qint32 _status) {
	return tr("No error");
}

QString Resources::statusInfo() {
	return tr("No error");
}

CResourceCache* Resources::cache() const {
	return m_cache;
}

Hues* Resources::hues() const {
	return m_hues;
}

CArts* Resources::arts() const {
	return m_arts;
}

IAnimation* Resources::getAnimation(quint32 _id, quint8 _action, quint8 _direction, quint32 _hue, bool _partialHue ) {
	return m_animations->getAnimation( _id, _action, _direction, _hue, _partialHue );
}

ITexture* Resources::getLandTexture(quint32 _id, quint32 _hue, bool _partialHue ) {
	return m_textures->getTexture( _id, _hue, _partialHue );
}

ITexture* Resources::getLandArt(quint32 _id, quint32 _hue, bool _partialHue) {
	return m_arts->getLandTile( _id, _hue, _partialHue );
}

ITexture* Resources::getStaticArt(quint32 _id, quint32 _hue, bool _partialHue) {
	return m_arts->getStaticTile( _id, _hue, _partialHue );
}

QImage* Resources::getCursor( qint32& _hotX, qint32& _hotY, quint32 _id, quint32 _hue, bool _partialHue ) {
	return m_arts->getCursor( _hotX, _hotY, _id, _hue, _partialHue );
}

ITexture* Resources::getGump( quint32 _id, quint32 _hue, bool _partialHue ) {
	return m_gumps->getGump( _id, _hue, _partialHue );
}

IMapBlock* Resources::getMapBlockByFacet(quint8 _facet, quint32 _id) {
	quint8 activeId_ = m_maps->active()->id;
	if( activeId_ != _facet ) {
		m_maps->switchFacet( _facet );
		IMapBlock* result = m_maps->getMapBlock( _id );
		m_maps->switchFacet( activeId_ );
		return result;
	} else {
		return getMapBlockFromActiveFacet( _id );
	}
}

IMapBlock* Resources::getMapBlockByFacet(quint8 _facet, quint32 _x, quint32 _y) {
	quint8 activeId_ = m_maps->active()->id;
	if( activeId_ != _facet ) {
		m_maps->switchFacet( _facet );
		IMapBlock* result = m_maps->getMapBlock( _x, _y );
		m_maps->switchFacet( activeId_ );
		return result;
	} else {
		return getMapBlockFromActiveFacet( _x, _y );
	}
}

IMapBlock* Resources::getMapBlockFromActiveFacet(quint32 _id) {
	return m_maps->getMapBlock( _id );
}

IMapBlock* Resources::getMapBlockFromActiveFacet( quint32 _x, quint32 _y) {
	return m_maps->getMapBlock( _x, _y );
}