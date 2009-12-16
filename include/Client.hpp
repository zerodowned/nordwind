#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <qapplication.h>
#include <qsettings.h>
#include "core/Resources.hpp"
#include "core/UserInterface.hpp"

/** Client
 *
*/
class Client : public QApplication {
	Q_OBJECT;
	public:
		Client( int argc, char** argv );
		virtual ~Client();

		bool load();

		static Client* getInstance();

		enum ClientMode {
			Standard,
			MapMaster,
			MulMaster,
			Developer
		};
		QPointer<core::UserInterfaces> mUserInterfaces;
		QPointer<resource::Resources> mResources;
};

inline Client* Client::getInstance() {
	return qobject_cast<Client*>(qApp);
}
#endif
