#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <qapplication.h>
#include <qpointer.h>
#include <qabstractsocket.h>

class QMainWindow;
/** Client
 *
*/
class Client : public QApplication {
	Q_OBJECT;
	public:
		Client(int& argc, char** argv);
		virtual ~Client();
		const QString& account() const;

		bool initialize();

		enum Privilegs {
			Player		=	1 << 0,
			Gamemaster	=	1 << 1,
			Designer	=	1 << 2,
			Developer	=	1 << 3,
			God			=	1 << 4
		};

		void addChild( QWidget* child, Qt::WindowFlags windowFlags = 0 );
        Q_SIGNALS:
                void login(const QString& account, const QString& password);
	public Q_SLOTS:
		void cleanup();
		void setAccount( const QString& account);
		void setPassword( const QString& password);
                void connectionEstablished();
                void error();
                void networkState(QAbstractSocket::SocketState socketState);
	private:
		Privilegs mPrivilegs;
		QString mAccount;
		QString mPassword;
		QPointer<QMainWindow> mGUI;
};
#endif
