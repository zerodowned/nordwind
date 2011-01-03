#include "ViewWidget.hpp"
#include <QGLWidget>
#include <QAction>

using namespace gui;

ViewWidget::ViewWidget(QGraphicsScene* scene, QWidget* parent)
    : QGraphicsView(scene,parent) {
    setObjectName("View");
    setRubberBandSelectionMode(Qt::ContainsItemShape);
    resize(QSize(640,480));

    setViewport(new QGLWidget);
    setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing|QGraphicsView::DontSavePainterState );
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
    setCacheMode(QGraphicsView::CacheBackground);

    setBackgroundBrush(Qt::black);
    /*QRadialGradient radialGradient(viewport()->rect().center(),
                                       viewport()->size().width()/2,
                                       viewport()->rect().bottomLeft());
        radialGradient.setColorAt(0.0f, Qt::white);
        radialGradient.setColorAt(1.0f, Qt::black);
        setForegroundBrush(radialGradient);*/
    QAction* toggleMap = new QAction("Toggle map grid",this);
    toggleMap->setCheckable(true);
    connect(toggleMap,SIGNAL(toggled(bool)),SLOT(toggleMap(bool)));
    addAction(toggleMap);

    setContextMenuPolicy(Qt::ActionsContextMenu);
    show();
}

ViewWidget::~ViewWidget() {
}

void ViewWidget::moveView(QPoint _position, qint16 _z) {
}

void ViewWidget::toggleMap(bool enable) {
    if(scene()) {
        scene()->setProperty("map.lines",enable);
        scene()->update();
    }
}
