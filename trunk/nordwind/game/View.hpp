#ifndef VIEW_HPP_
#define VIEW_HPP_

#include <QtGui/qgraphicsview.h>

namespace game {
	class View : public QGraphicsView {
		Q_OBJECT;
		public:
			View( QWidget* _parent );
//			void setScene( QPointer<Scene> _scene );
//			void setSceneRect( qreal _x, qreal _y, qreal _z, qreal _width, qreal _height );
			virtual ~View();
		Q_SLOTS
			void moveView( QPoint _position, qint16 _z );
	};
}
#endif
