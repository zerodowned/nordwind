#include "CResourceCache.h"
#include "CResourceHandler.h"

using namespace resource;

CResourceCache* CResource::cache = NULL;

CResource::CResource(QString _key)
    : m_refcount(1), m_key(_key) {
    cache->registerResource(this);
}

CResource::~CResource() {
    cache->unregisterResource(this);
}

void CResource::incRef() {
    m_refcount++;
}

void CResource::decRef() {
    if(--m_refcount==0) {
        cache->beforeTextureRemove(this);
    }
}

QString& CResource::key() {
	return m_key;
}

bool CResource::cacheable() {
    return true;
}

CResourceCache::CResourceCache() {
	m_finish = false;
	quint32 cachesize = Resources::instance().settings()->value("cache", 100).toUInt();
	m_resourceCache.setMaxCost(cachesize);
	CResource::cache = this;
}

CResourceCache::~CResourceCache() {
	m_finish = true;
    m_resourceCache.clear();
    foreach( CResource* _res, m_resourceMap.values() ) {
        delete _res;
    };
	CResource::cache = NULL;
}

void CResourceCache::registerResource( CResource* _this ) {
     QString key = _this->key();
     m_resourceMap[key] = _this;
}

void CResourceCache::unregisterResource( CResource* _this ) {
	if(m_finish)
		return;
    if(m_resourceMap.remove( _this->key() )>=1)
        return;
	return;
}

void CResourceCache::beforeTextureRemove( CResource* _this ) {
	QString key = _this->key();
	if( m_resourceMap.contains( key ) ) {
		m_resourceMap.remove( key );
		 m_resourceCache.insert( key, _this );
	} else {
		delete _this;
	}
}

CResource* CResourceCache::available( QString _key ) {
    CResource* result = NULL;
    if( m_resourceMap.contains( _key ) ) {
        result = m_resourceMap[_key];
		result->incRef();
		return result;
    }
    if( result = m_resourceCache.take( _key ) ) {
        m_resourceMap[_key] = result;
		result->incRef();
		return result;
    }
	return NULL;
}

void CResourceCache::insert( CResource* _this ) {
    if(!_this)
        return;
    m_resourceMap[_this->key()] = _this;
}

QString CResourceCache::status() {
    return QString("Currently there are %1 elements in the map and %2 are cached").arg(m_resourceMap.count()).arg(m_resourceCache.count());
}
