#include "CWorldBlock.h"
#include "../core/Client.h"

using namespace world;

CWorldBlock::CWorldBlock( QGraphicsItem* parent ) 
: QGraphicsItem( parent), m_data(NULL) {
	
}


CWorldBlock::~CWorldBlock()	{
}

QRectF CWorldBlock::boundingRect() const {
	return QRectF( 0,0, m_data->pos().width(),m_data->pos().height() );
}

void CWorldBlock::paint(QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget) {
	if(!m_data)
	return;
}


