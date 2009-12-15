#include "CBorderGump.h"
#include <Qt/QPainter.h>
#include "../CClient.h"
#include "../resource/CGumps.h"

using namespace gui;

void CBorderGump::loadGumps() {
	for( quint8 i = 0; i < 9; i++ ) {
		if(m_gumps[i])
			m_gumps[i]->decRef();
		m_gumps[i] = resource::Gumps::instance().getGump( m_ids[i], m_hue, m_partialHue ); 
		if(!m_gumps[i])
			m_valid = false;
	}
}

void CBorderGump::updateMask() {
	if(!m_valid)
		return;
	QRegion mask(m_gumps[8]->mask());
	mask.translate( m_vectors[10] );
	mask += QRegion( m_gumps[6]->mask() ).translated( m_vectors[8] );
	mask += QRegion( m_gumps[2]->mask() ).translated( m_vectors[2] );
	mask += QRegion( m_gumps[0]->mask() );
	mask += QRegion( QRect( m_vectors[5], m_vectors[10] ) );
	
	qint32 mwidth = m_vectors[2].x()-m_vectors[1].x();
	qint32 mheight = m_vectors[8].y()-m_vectors[4].y();
	for(qint32 mx = m_vectors[1].x(); mx < mwidth; mx+= m_gumps[1]->image().width() ) {
		mask += QRegion( m_gumps[1]->mask() ).translated( mx,0 ).intersected( QRect( m_vectors[1], m_vectors[6] ) );
		mask += QRegion( m_gumps[7]->mask() ).translated( mx,m_vectors[9].y() ).intersected( QRect( m_vectors[9], m_vectors[14] ) );
	}
	for(qint32 my = m_vectors[4].y(); my < mheight; my+= m_gumps[3]->image().height() ) {
		mask += QRegion( m_gumps[3]->mask() ).translated( 0,my ).intersected( QRect( m_vectors[4], m_vectors[9] ) );
		mask += QRegion( m_gumps[5]->mask() ).translated( m_vectors[6].x(),my ).intersected( QRect( m_vectors[6], m_vectors[11] ) );
	}
	setMask( mask );
}

void CBorderGump::calcVectors( quint16 _width, quint16 _height ) {
	if(!m_valid)
		return;
	m_vectors[0] = QPoint( 0,0 );
	m_vectors[1] = QPoint( m_gumps[0]->image().width(), 0 );
	m_vectors[2] = QPoint( _width-m_gumps[2]->image().width(), 0 );
	m_vectors[3] = QPoint( _width, 0 );
	m_vectors[4] = QPoint( 0,m_gumps[0]->image().height() );
	m_vectors[5] = QPoint( m_gumps[0]->image().width(), m_gumps[0]->image().height() );
	m_vectors[6] = QPoint( _width-m_gumps[0]->image().width(), m_gumps[1]->image().height() );
	m_vectors[7] = QPoint( _width, m_gumps[2]->image().height() );
	m_vectors[8] = QPoint( 0,_height-m_gumps[6]->image().height() );
	m_vectors[9] = QPoint( m_gumps[6]->image().width(), _height-m_gumps[6]->image().height() );
	m_vectors[10] = QPoint( _width-m_gumps[8]->image().width(), _height-m_gumps[8]->image().height() ); 
	m_vectors[11] = QPoint( _width, _height-m_gumps[8]->image().height() ); 
	m_vectors[12] = QPoint( 0,_height );
	m_vectors[13] = QPoint( m_gumps[6]->image().width(), _height ); 
	m_vectors[14] = QPoint( _width-m_gumps[8]->image().width(), _height );
	m_vectors[15] = QPoint( _width, _height );
}

CBorderGump::CBorderGump( QWidget* _parent, qint32 _x, qint32 _y, quint16 _width, quint16 _height, quint32 _id, quint32 _hue, bool _partialHue, const QString& _name) 
: QGLWidget( _parent, Client::instance().gui() ), m_hue(_hue), m_partialHue(_partialHue), m_valid(true) {
	if(_name!="")
		setObjectName(_name);
	move( _x, _y );
	for(quint8 i=0; i< 9; i++ ) {
		m_gumps[i] = 0;
		m_ids[i] = _id-4+i;
	}
	loadGumps();
	resize( _width, _height );
}

CBorderGump::~CBorderGump() {
	for(quint8 i = 0; i < 9; i++ ) {
		if(m_gumps[i])
			m_gumps[i]->decRef();
	}
}

void CBorderGump::setIds( quint32 _topleft, quint32 _top, quint32 _topright, quint32 _left, quint32 _center, quint32 _right, quint32 _bottomleft, quint32 _bottom, quint32 _bottomright ) {
	m_ids[0] = _topleft;
	m_ids[1] = _top;
	m_ids[2] = _topright;
	m_ids[3] = _left;
	m_ids[4] = _center;
	m_ids[5] = _right;
	m_ids[6] = _bottomleft;
	m_ids[7] = _bottom;
	m_ids[8] = _bottomright;
	loadGumps();
	calcVectors( width(), height() );
	update();
}

void CBorderGump::setId( quint32 _center ) {
	if(_center==m_ids[4])
		return;
	for(quint8 i=0; i< 9; i++ ) {
		m_ids[i] = _center-4+i;
	}
	loadGumps();
	calcVectors( width(), height() );
	update();
}

quint32 CBorderGump::id() const {
	return m_ids[4];
}

void CBorderGump::setHue( quint32 _hue ) {
	if(_hue==m_hue)
		return;
	m_hue = _hue;
	loadGumps();
	update();
}

quint32 CBorderGump::hue() const {
	return m_hue;
}

void CBorderGump::setPartialHue( bool _partialHue ) {
	if(m_partialHue==_partialHue)
		return;
	m_partialHue = _partialHue;
	loadGumps();
	update();
}

bool CBorderGump::partialHue() const {
	return m_partialHue;
}

QPoint& CBorderGump::vector( quint8 _i ) {
	if( _i > 15 || !m_valid )
		return QPoint();
	return m_vectors[_i];
}

void CBorderGump::resize(qint32 _w, qint32 _h ) {
	QGLWidget::resize( _w, _h );
	if(!m_valid)
		return;
	quint16 topanchorswidth = m_gumps[0]->image().width()+m_gumps[1]->image().width();
	quint16 leftanchorsheight =   m_gumps[0]->image().height()+m_gumps[2]->image().height();
	if(topanchorswidth>width() ||
		leftanchorsheight>height() ) {
		return;
	}
	calcVectors( _w, _h );
	updateMask();
}

quint16 CBorderGump::anchorsWidth() const {
	return m_vectors[1].x()+width()-m_vectors[2].x();
}

quint16 CBorderGump::anchorsHeight() const {
	return m_vectors[4].y()+height()-m_vectors[8].y();
}

void CBorderGump::paintEvent( QPaintEvent* _event ) {
    Q_UNUSED(_event);
	QPainter painter(this);
	if(m_valid) {
		quint16 topanchorswidth = m_gumps[0]->image().width()+m_gumps[2]->image().width();
		quint16 leftanchorsheight =   m_gumps[0]->image().height()+m_gumps[6]->image().height();
		if(topanchorswidth>width() ||
			leftanchorsheight>height() ) {
			quint16 mwidth = width()/2;
			quint16 mheight = height()/2;
			// top left
			painter.drawImage( QRect( 0,0, mwidth, mheight), m_gumps[0]->image() );
			// top right
			painter.drawImage( QRect( mwidth,0, mwidth, mheight), m_gumps[2]->image() );
			// bottom left
			painter.drawImage( QRect( 0,mheight, mwidth, mheight), m_gumps[6]->image() );
			// bottom right
			painter.drawImage( QRect( mwidth,mheight, mwidth, mheight), m_gumps[8]->image() );
		} else {

			// top
			painter.drawTiledPixmap( QRect( m_vectors[1], m_vectors[6] ), QPixmap::fromImage( m_gumps[1]->image() ) );
			// left
			painter.drawTiledPixmap( QRect( m_vectors[4], m_vectors[9] ), QPixmap::fromImage( m_gumps[3]->image() ) );
			// rigth
			painter.drawTiledPixmap( QRect( m_vectors[6], m_vectors[11] ), QPixmap::fromImage( m_gumps[5]->image() ) );
			// bottom
			painter.drawTiledPixmap( QRect( m_vectors[9], m_vectors[14] ), QPixmap::fromImage( m_gumps[7]->image() ) );
			// center
			painter.drawTiledPixmap( QRect( m_vectors[5], m_vectors[10] ), QPixmap::fromImage( m_gumps[4]->image() ) );
			// top left
			painter.drawImage( m_vectors[0], m_gumps[0]->image() );
			// top right
			painter.drawImage( m_vectors[2], m_gumps[2]->image() );
			// bottom left
			painter.drawImage( m_vectors[8], m_gumps[6]->image() );
			// bottom right
			painter.drawImage( m_vectors[10], m_gumps[8]->image() );
		}
	} else {
		painter.setPen(  QPen(Qt::red, 1, Qt::SolidLine) );
		QVector<QPoint> points(5);
		points << QPoint(0,0) 
			<< QPoint(width()-1,height()-1) 
			<< QPoint(width()-1, 0) 
			<< QPoint(0,0) 
			<< QPoint( 0, height()-1 )
			<< QPoint(width()-1,height()-1);
		painter.drawPolygon( points.data(), points.size() );
	}
	painter.end();
}