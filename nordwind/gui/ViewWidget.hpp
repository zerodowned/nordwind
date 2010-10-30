#ifndef VIEW_HPP_
#define VIEW_HPP_

#include <qgraphicsview.h>

namespace game {
	class Mobile;
}

namespace gui {
	class ViewWidget : public QGraphicsView {
		Q_OBJECT;
		public:
			ViewWidget( const QString& facteName, const QPoint& offset, const QSize& size, QWidget* _parent = NULL );
//			void setScene( QPointer<Scene> _scene );
//			void setSceneRect( qreal _x, qreal _y, qreal _z, qreal _width, qreal _height );
			virtual ~ViewWidget();
		Q_SLOTS
			void moveView( QPoint _position, qint16 _z );
		protected:
			game::Mobile* mCharacter;
	};
}
#endif

