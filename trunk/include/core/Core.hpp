#ifndef _CCORE_H__
#define _CCORE_H__

#include <qstatemachine.h>
#include <qhash.h>

namespace core {
	class Core : public QStateMachine {
            Q_OBJECT;
            public:
				Core(QObject* _parent = NULL);
				~Core(void);
				void startup();
				void shutdown();
				void reload();
				Module* getModuleByName( QString _name ) const;
				Module* getModuleByName( QString _name );
            private:
				QHash< QString, Module* > mModules;
	};
}

#endif
