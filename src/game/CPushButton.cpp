#include "CPushButton.h"
#include "../resource/CGumps.h"
#include "../CClient.h"
#include <Qt/QPainter.h>
#include <Qt/QEvent.h>
#include <Qt/QBitmap.h>
using namespace gui;

resource::CGump* CPushButton::getCurrent() const {
	if(m_stateGumps.contains( isState() )) {
		return m_stateGumps[isState()];
	} else {
		return m_stateGumps[eBtnStandart];
	}
}

void CPushButton::buildConnections() {
	connect( this, SIGNAL(leave()), this, SLOT(onLeave()) );
	connect( this, SIGNAL(hovered()), this, SLOT(onHovered()) );
	connect( this, SIGNAL(pressed()), this, SLOT(onPressed()) );
	connect( this, SIGNAL(released()), this, SLOT(onReleased()) );
	connect( this, SIGNAL(click()), this, SLOT(onClick()) );	
}

CPushButton::CPushButton( QWidget* _parent, qint32 _x, qint32 _y, const QString& _str )
: QGLWidget( _parent, Client::instance().gui() ), m_state( eBtnStandart ) {
	if(_str!="")
		setObjectName(_str);
	move( _x,_y );
	connect( this, SIGNAL(stateChanged()), this, SLOT(onStateChanged()) );
	buildConnections();
}

CPushButton::~CPushButton() {
	foreach( resource::CGump* gump, m_stateGumps.values() ) {
		gump->decRef();
	}
}

void CPushButton::paintEvent(QPaintEvent* _event ) {
    Q_UNUSED(_event);
	QPainter painter(this);
	resource::CGump* gump = getCurrent();
	painter.drawImage(QPoint(0,0), gump->image(), gump->image().rect() );
	painter.end();
}

void CPushButton::enterEvent ( QEvent * event ) {
	emit hovered();
}

void CPushButton::leaveEvent ( QEvent * event ) {
	emit leave();
}

void CPushButton::mousePressEvent ( QMouseEvent * event ) {
	if(event->button()==Qt::LeftButton) {
		emit pressed();
	};
}

void CPushButton::mouseReleaseEvent ( QMouseEvent * event ) {
	if(event->button()==Qt::LeftButton) {
		if(m_isPressed&&rect().contains(event->x(),event->y())) {
			emit click();
		};
	};
	emit released();
}

CPushButton& CPushButton::addState( eButtonState _state, quint32 _id, quint32 _hue, bool partialHue ) {
	resource::CGump* gump = resource::Gumps::instance().getGump( _id, _hue, partialHue );
	if(!gump)
		return *this;
	if(m_stateGumps.contains( _state ))
		m_stateGumps[_state]->decRef();

	if(m_stateGumps.empty()) {
		m_stateGumps[eBtnStandart] = gump;
		setMask(gump->mask());
	}
	if(_state==eBtnHovered)
		setMouseTracking( true );
	m_stateGumps[_state] = gump;
	update();
	return *this;
}

void CPushButton::setState( eButtonState _newState ) {
	m_state = _newState;
	emit stateChanged();
	update();
}

inline eButtonState CPushButton::isState() const {
	return m_state;
}

void CPushButton::onStateChanged() {
	resource::CGump* gump = getCurrent();
	resize( gump->image().width(),gump->image().height());
	setMask(gump->mask());
}

void CPushButton::onLeave() {
	setState( eBtnStandart );
	m_isPressed = false;
}

void CPushButton::onClick() {
	setState( eBtnClicked );
}

void CPushButton::onPressed() {
	setState( eBtnPressed );
	m_isPressed = true;
}

void CPushButton::onHovered() {
	setState( eBtnHovered );
}

void CPushButton::onReleased() {
	setState( eBtnHovered );
	m_isPressed = false;
}