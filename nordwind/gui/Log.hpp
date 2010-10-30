#ifndef LOG_HPP__
#define LOG_HPP__

#include <qobject.h>
#include "Singleton.hpp"

#include "../util/DebugInformation.hpp"

namespace core {
        class Log : public QObject, public Singleton<core::Log> {
			friend class Singleton<core::Log>;
			Q_OBJECT;
			private:

				Log();
				~Log();
			public:
				class Entry {
					public:
						enum Type {
							Debug = 0,
							Warning = 1,
							Critical_Error = 2,
							Fatal_Error = 3,
							System_Info = 3,
							Error,
							Info
						};
						Type getType();
						QByteArray getMessage();
						DebugInformation& getDebugInformation();
						Entry( Type _type, QByteArray _message, DebugInformation _debuginformation );
						~Entry();
				}
				void log( Entry );
        };
}

#define NOTICE(x) core::Log::instance().printToLog( Log::Info, x, DEBUGINFORMATION );
#define INFO(x) core::Log::instance().log( QString("%1 || + %2\n").arg( QTime::currentTime().toString( Qt::ISODate ) ).arg(x) );
#define WARNING(x)  core::CLog::instance().log( QString("%1 || ! %2\n").arg(x).arg( QTime::currentTime().toString( Qt::ISODate ) ).arg(x) );
#define ERR(x, type)  core::CLog::instance().error( QString("%1 || Error %2\n         ||       %3\n").arg( QTime::currentTime().toString( Qt::ISODate ) ).arg(#type).arg(x) );
#define CRIT_ERR(x)  core::CLog::instance().error( QString("%1 || !!! critical Error in %2 at line %3\n         ||       %4\n").arg( QTime::currentTime().toString( Qt::ISODate ) ).arg(__FILE__).arg(__LINE__).arg(x) );


#endif
