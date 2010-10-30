#ifndef CSOCKET_H_
#define CSOCKET_H_

#include <qtcpsocket.h>
#include <qscopedpointer.h>

namespace network {
	class Socket : public QTcpSocket {
	Q_OBJECT;
	public:
		typedef QByteArray Packet;
		class Operator {
			public:
				virtual ~Operator() {};
				virtual void in(QByteArray& inbound) {
					Q_UNUSED(inbound);
				};
				virtual void out(QByteArray& outbound) {
					Q_UNUSED(outbound);
				};
		};
        protected:
		quint32 mSeed;
		QScopedPointer<Operator> mEncryption;
		QScopedPointer<Operator> mCompression;
	public:
                Socket(const QString& host, quint16 port, quint32 seed = qrand());
		virtual ~Socket();
	protected:
		qint64 writeData(const char* data, qint64 maxSize);
		qint64 readData(char* data, qint64 maxSize);
	Q_SIGNALS:
                void readyWrite();
	protected Q_SLOTS:
                void establishConnection();
		virtual void dispatch() = 0;
		void sendHeader();
	};
}
#endif
