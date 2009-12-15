#ifndef CRESOURCECACHE_H
#define CRESOURCECACHE_H

#include <Qt/QHash.h>
#include <Qt/QCache.h>
#include <Qt/QString.h>
#include <Qt/QObject.h>

namespace resource {
	class CResourceCache;
    class CResource {
        private:
            quint32 m_refcount;
			QString m_key;
			CResource( CResource& _r ) {};
        public:
            CResource(QString _key = "Invalid");
            virtual ~CResource();
            void incRef();
            void decRef();
            QString& key();
            bool cacheable();
			static CResourceCache* cache;
    };

	class CResourceCache {
        private:
			bool m_finish;
            QHash<QString, CResource*> m_resourceMap;
            QCache<QString, CResource> m_resourceCache;
        public:
			CResourceCache( );
            ~CResourceCache();
            void registerResource( CResource* _this );
            void unregisterResource(CResource* _this );
            void beforeTextureRemove(CResource* _this );
            CResource* available( QString _key );
            void insert( CResource* _this );
            QString status();
    };
}
#endif
