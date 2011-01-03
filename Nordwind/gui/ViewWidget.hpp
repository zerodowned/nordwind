#ifndef VIEW_HPP_
#define VIEW_HPP_

#include <qgraphicsview.h>

namespace gui {
        class ViewWidget : public QGraphicsView {
                Q_OBJECT;
                public:
                    ViewWidget( QGraphicsScene* scene = NULL, QWidget* parent = NULL );
//                  void setScene( QPointer<Scene> _scene );
//                  void setSceneRect( qreal _x, qreal _y, qreal _z, qreal _width, qreal _height );
                    virtual ~ViewWidget();
                public Q_SLOTS:
                    void moveView( QPoint _position, qint16 _z );
                    void toggleMap(bool enable);
        };
}
#endif

