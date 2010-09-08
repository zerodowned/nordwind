#include "View.hpp"

using namespace game;

View::View(QWidget* _parent)
: QGraphicsView(_parent) {
	setOptimizationFlags( QGraphicsView::DontAdjustForAntialiasing|QGraphicsView::DontSavePainterState );
	setResizeAnchor(QGraphicsView::AnchorViewCenter);
	setRubberBandSelectionMode(Qt::ContainsItemShape);
//	setBackgroundBrush(Qt::black);
}

View::~View() {
}
