#include "CGuiImage.h"
#include <Qt/QPainter.h>
#include "../CClient.h"
#include "../resource/CGumps.h"

using namespace gui;

void CGumpImage::buildConnections() {
	connect( this, SIGNAL(idChange()), this, SLOT(onIdChange()) );
	connect( this, SIGNAL(hueChange()), this, SLOT(onHueChange()) );
	connect( this, SIGNAL(gumpChange()), this, SLOT(onGumpChange()) );
	connect( this, SIGNAL(resizeChange()), this, SLOT(onResizeChange()) );
	connect( this, SIGNAL(partialHueChange()), this, SLOT(onPartialHueChange()) );
}

CGumpImage::CGumpImage( QWidget* _parent, qint32 _x, qint32 _y, quint32 _id, quint32 _hue, bool _partialHue, const QString& _name, quint16 _width, quint16 _height, bool _resize, bool _tiled )
: QGLWidget( _parent, Client::instance().gui() ), m_gump(NULL), m_id(_id), m_hue(_hue), m_partialHue(_partialHue), m_resize(_resize), m_tiled(_tiled) {
	if(_name!="")
		setObjectName(_name);
	move( _x, _y );
	quint16 width = (_width!=0xFFFF) ? _width : 50;
	quint16 height = (_height!=0xFFFF) ? _height : 50;
	m_gump = resource::Gumps::instance().getGump( _id, _hue, _partialHue );
	if(m_gump) {
		if(_resize) {
			width = m_gump->image().width();
			height = m_gump->image().height();
		}
		if(!m_tiled)
			setMask( m_gump->mask() );
	}
	resize( width, height );
	buildConnections();
}

CGumpImage::~CGumpImage() {
	if(m_gump)
		m_gump->decRef();
}

quint32 CGumpImage::id() const {
	return m_id;
}

void CGumpImage::setId(quint32 _id ) {
	if(m_id==_id)
		return;
	if(m_gump)
		m_gump->decRef();
	m_gump = resource::Gumps::instance().getGump( _id, m_hue, m_partialHue );
	if(m_gump&&m_resize) {
		resize( m_gump->image().width(), m_gump->image().height() );
	}
	m_id = _id;
	emit gumpChange();
	emit idChange();
}

quint32 CGumpImage::hue() const {
	return m_hue;
}

void CGumpImage::setHue(quint32 _hue) {
	if(m_hue==_hue)
		return;
	if(m_gump)
		m_gump->decRef();
	m_gump = resource::Gumps::instance().getGump( m_id, _hue, m_partialHue );	
	m_hue = _hue;
	emit gumpChange();
	emit hueChange();
}

bool CGumpImage::partialHue() const {
	return m_partialHue;
}

void CGumpImage::setPartialHue(bool _partialHue) {
	if(m_partialHue==_partialHue)
		return;
	if(m_gump)
		m_gump->decRef();
	m_gump = resource::Gumps::instance().getGump( m_id, m_hue, _partialHue );
	m_partialHue = _partialHue;
	emit gumpChange();
	emit partialHueChange();
}

void CGumpImage::setType(quint32 _id, quint32 _hue, bool _partialHue, bool _resize ) {
	if(m_id==_id&&_hue==m_hue&&_partialHue==m_partialHue)
		return;
	if(m_gump)
		m_gump->decRef();
	m_gump =  resource::Gumps::instance().getGump( _id, _hue, _partialHue );
	if(m_gump&&_resize) {
		resize( m_gump->image().width(), m_gump->image().height() );
	}
	emit gumpChange();
	if(m_id!=_id) {
		m_id = _id;
		emit idChange();
	}
	if(m_hue!=_hue) {
		m_hue = _hue;
		emit hueChange();
	}
	if(m_partialHue!=_partialHue) {
		m_partialHue = _partialHue;
		emit partialHueChange();
	}
	if(m_resize!=_resize) {
		m_resize=_resize;
		emit resizeChange();
	}
}



bool CGumpImage::isResize() const {
	return m_resize;
}

void CGumpImage::setResize( bool _resize ) {
	if(m_resize==_resize)
		return;
	if(m_gump&&_resize) {
		resize( m_gump->image().width(), m_gump->image().height() ); 
	}
	emit resizeChange();
}

bool CGumpImage::isTiled() const {
	return m_tiled;
}

void CGumpImage::setTiled(bool _tile) {
	if(m_tiled==_tile)
		return;
	m_tiled = _tile;
}

void CGumpImage::onGumpChange() {
	if(m_gump&&!m_tiled)
		setMask(m_gump->mask());
	update();
}

void CGumpImage::onHueChange() {
}

void CGumpImage::onIdChange() {
}

void CGumpImage::onResizeChange() {
}

void CGumpImage::onPartialHueChange() {
}


void CGumpImage::paintEvent(QPaintEvent *_event) {
    Q_UNUSED(_event);
	QPainter painter(this);
	if(m_gump) {
		if(m_tiled) {
			//painter.drawImage( QRect(0,0, m_gump->image().width(), m_gump->image().height()), m_gump->image() );
			//painter.drawImage( QRect(width()-m_gump->image().width(),height()-m_gump->image().height(), m_gump->image().width(), m_gump->image().height()), m_gump->image() );
			painter.drawTiledPixmap( 0,0, width(), height(), QPixmap::fromImage( m_gump->image() ) );			
		} else {
			painter.drawImage( QRect(0,0, width(), height()), m_gump->image() );
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