#ifndef CCLIENT_H
#define CCLIENT_H

#include <qapplication.h>
#include <qmap.h>
#include <qsettings.h>

/** Client
 *
*/
class Client : public QApplication {
	Q_OBJECT;
	public:
		Client( int argc, char** argv );
		virtual ~Client();

		const Client* getInstance() const;
		Client* getInstance();

		const QString& version();

		enum eClientState {
			eCS_Start = 1 << 0,
			eCS_InitCore = 1 << 1,
			eCS_LoadingCore = 1 << 2,
			eCS_Core = 1 << 3,
			eCS_InitGui = 1 << 4,
			eCS_LoadingGui = 1 << 5,
			eCS_Gui = 1 << 6,
			eCS_InitSound = 1 << 7,
			eCS_LoadingSound = 1 << 8,
			eCS_Sound = 1 << 9,
			eCS_InitGame = 1 << 10,
			eCS_LoadingGame = 1 << 11,
			eCS_Game = 1 << 12,
			eCS_InitNetwork = 1 << 13,
			eCS_LoadingNetwork = 1 << 14,
			eCS_Network = 1 << 15,
			eCS_ShutDown = 1 << 16
			/*
			eCS_InitMapMaster = 1 << 17,
			eCS_LoadingMapMaster = 1 << 18,
			eCS_MapMaster = 1 << 19,
			eCS_InitMulMaster = 1 << 20,
			eCS_LoadingMulMaster = 1 << 21,
			eCS_MulMaster = 1 << 22,
			eCS_InitDev = 1 << 23,
			eCS_LoadingDev = 1 << 24,
			eCS_Dev = 1 << 25 */
		};
	private:
		QSettings* mSettings;
};

inline const Client* Client::getInstance() const {
	return qobject_cast<const Client*>(qApp);
}

inline Client* Client::getInstance() {
	return qobject_cast<Client*>(qApp);
}
#endif
