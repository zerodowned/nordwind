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

		QSharedPointer<core::UserInterfaces> userInterfaces();
		QSharedPointer<resource::Resources> resources();
		QSharedPointer<QScriptEngine> scriptEngine();
		const QSettings& settings() const;
		QSettings& settings();
	private:
		QSettings mSettings;
		QSharedPointer<core::UserInterfaces> mUserInterfaces;
		QSharedPointer<resource::Resources> mResources;
		QSharedPointer<QScriptEngine> mScriptEngine;
};

inline QSharedPointer<core::UserInterfaces> Client::userInterfaces() {
	return mUserInterfaces;
}

inline QSharedPointer<resource::Resources> Client::resources() {
	return mResources;
}

inline QSharedPointer<QScriptEngine> Client::scriptEngine() {
	return mScriptEngine;
}

inline Client* Client::getInstance() {
	return qobject_cast<Client*>(qApp);
}

inline QSettings& Client::settings() {
	return mSettings;
}

inline const QSettings& Client::settings() const {
	return mSettings;
}
#endif
