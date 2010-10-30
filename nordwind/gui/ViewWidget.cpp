#include "ViewWidget.hpp"
#include <QGLWidget>
#include "../game/Scene.hpp"
#include "../game/Mobile.hpp"
#include <QVector2D>
#include <qdatetime.h>
#include <qdebug.h>

using namespace gui;

ViewWidget::ViewWidget(const QString& facetName, const QPoint& offset, const QSize& size, QWidget* parent)
: QGraphicsView(parent),
mCharacter(new game::Mobile) {
	setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing|QGraphicsView::DontSavePainterState );
	setResizeAnchor(QGraphicsView::AnchorViewCenter);
//  setRubberBandSelectionMode(Qt::ContainsItemShape);
	setBackgroundBrush(Qt::black);
//	setViewport(new QGLWidget);
	game::Scene* scene = game::Scene::instance(facetName);
	if(scene) {
		QTime timer;
		timer.start();
		QRect mapRect(offset,size);
		scene->loadMap(mapRect);
		qDebug() << "Load done in" << timer.restart() << "ms";
		scene->addItem(mCharacter,QVector3D(QVector2D(mapRect.center()),20));
		setScene(scene);
		centerOn(mCharacter);
	}
}

ViewWidget::~ViewWidget() {
}
