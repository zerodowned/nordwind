#ifndef CWORLDVIEW_H_
#define CWORLDVIEW_H_

#include <Qt/QGraphicsView.h>

namespace gui {
	class CWorldView : public QGraphicsView
	{
		Q_OBJECT
	private:
		bool m_playerWorld;
	public:
		CWorldView(quint8 _facet, QSize _size, QWidget* parent = 0);
		CWorldView(quint8 _facet, QPoint _center, QSize _size, QWidget* parent = 0);
		CWorldView(quint8 _facet, QRect _dim, QWidget* parent = 0);
		virtual ~CWorldView();
	//protected:
	};
}
#endif