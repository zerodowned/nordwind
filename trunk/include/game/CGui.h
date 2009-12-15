#ifndef QGUICLIENT_H
#define QGUICLIENT_H

#include <Qt/QGraphicsView.h>
#include "../interfaces/IModule.h"
/*
	This class holds all gui elements and gives access to the sub
	views of the current game scene
	list of Gui-elements (dialogs)
	list of Views on Game (GameViews)
	Handels UserInput/Scripts
*/
namespace gui {
	class CCursor;
	class CGui : public QGraphicsView, public IModule {
	Q_OBJECT;
	private:
		// QMap<QString, CDialog*> m_dialogs;
		// CDialog* m_activedialog;
		// QList<CDialog> m_dialog_order;
		//quint32 fps;
		QGraphicsScene* m_scene;
		CCursor* m_cursor;
	public:
		CGui();
		virtual ~CGui();
		bool load();
		bool unload();
		bool reload();
		const QString& version();
		const QString name();
		qint32 status();
		QString statusDesc( qint32 _status );
		QString statusInfo();

		CCursor* cursor();
	protected:
		void closeEvent(QCloseEvent *event);
		//void resizeGL( qint32 _width, qint32 _height );
	};
	const QString guiVersion = "0.1 03/15/09";
}

#endif // QGUICLIENT_H
