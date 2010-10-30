#ifndef USERINTERFACE_HPP__
#define USERINTERFACE_HPP__

#include <QtGui/qmainwindow.h>
#include <QtCore/qpointer.h>

class QMdiArea;
class QMdiSubWindow;
/*
	This class holds all gui elements and gives access to the sub
	views of the current game scene
	list of Gui-elements (dialogs)
	list of Views on Game (GameViews)
	Handels UserInput/Scripts
*/
namespace core {
	class UserInterfaces : public QMainWindow {
		Q_OBJECT;
		private:
			// QMap<QString, CDialog*> m_dialogs;
			// CDialog* m_activedialog;
			// QList<CDialog> m_dialog_order;
			//quint32 fps;
	//		QGraphicsScene* mScene;
	//		CCursor* m_cursor;
		public:
			UserInterfaces();
			virtual ~UserInterfaces();
			typedef QPointer<QMdiSubWindow> UserInterface;
			UserInterface addUI( QWidget* _element );
		private:
			QPointer<QMdiArea> getUIArea();
	};
}

#endif // QGUICLIENT_H
