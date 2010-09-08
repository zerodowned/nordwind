#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <QtGui/qapplication.h>
#include <QtCore/qsettings.h>
#include <QtScript/qscriptengine.h>

namespace core {
	class UserInterfaces;
}

namespace resource {
	class Resources;
}
/** Client
 *
*/
class Client : public QApplication {
	Q_OBJECT;
	public:
		Client( int& argc, char** argv );
		virtual ~Client();

		bool load();
		bool unload();

		static Client* getInstance();

		enum ClientMode {
			Standard,
			MapMaster,
			MulMaster,
			Developer
		};

		core::UserInterfaces& userInterfaces();
		resource::Resources& resources();
		QScriptEngine& scriptEngine();
		static QSettings& settings();
	private:
		static QSettings sSettings;
		QScopedPointer<core::UserInterfaces> mUserInterfaces;
		QScopedPointer<resource::Resources> mResources;
		QScopedPointer<QScriptEngine> mScriptEngine;
};
#endif
