#include "CTextField.h"
#include "../CClient.h"
#include "CBorderGump.h"
#include "CLabel.h"
#include <QtGui/QKeyEvent>

using namespace gui;

CTextField::CTextField(QWidget* _parent, qint32 _x, qint32 _y, QString& _text, const QString& _name, bool _background, quint32 _backid, quint32 _backhue, quint32 _focusHue )
: QGLWidget(_parent, Client::instance().gui()), m_label(NULL), m_cursor(NULL), m_cursorPos(QPoint(0,0)), m_stdHue(0), m_focusHue(0) {
	move( _x, _y );
	m_refresher.setInterval( 200 );
	m_label = new CLabel( this, 0,0, _text, _name + ".Label" );
	if(m_label) {
		m_stdHue = m_label->hue();
		m_focusHue = _focusHue;
		setFocusPolicy( Qt::StrongFocus );
		if(m_background) {
			resize( m_label->size() );
			m_background->resize( width() , height() );
			m_background->update();
			m_label->move( m_background->vector(5) );
		} else {
			resize( m_label->size() );
			setMask( m_label->mask() );
		}
		connect( m_label, SIGNAL(textChange(const QString&)), this, SLOT(onTextChange(const QString&)) );
	}
	m_history.clear();
}

CTextField::~CTextField() {
	if(m_label)
		delete m_label;
	if(m_background)
		delete m_background;
}

bool CTextField::hasBackground() {
	return (m_background)?true:false;
}

void CTextField::setBackground( quint32 _id, quint32 _hue ) {
	if(hasBackground())
		delete m_background;
	m_background = new CBorderGump( this, 0, 0, width(), height(), _id, _hue );
	resize( m_label->width() + m_background->anchorsWidth(), m_label->height() + m_background->anchorsHeight() );
	m_background->resize( width(), height() );
	m_label->move( m_background->vector(5) );
}

quint32 CTextField::focusHue() const {
	return m_focusHue;
}

void CTextField::setFocusHue(quint32 _focusHue ) {
	m_focusHue = _focusHue;
}

void CTextField::focusInEvent( QFocusEvent * event ) {
	m_stdHue = m_label->hue();
	m_label->setHue( m_focusHue );
}

void CTextField::focusOutEvent( QFocusEvent * event ) {
	m_label->setHue( m_stdHue );
}

void CTextField::keyPressEvent( QKeyEvent* event ) {
	if(event->key()==Qt::Key_Backspace) {
		QString _text = m_label->text();
		if(_text.size()==1)
			_text = " ";
		_text.chop(1);
		m_label->setText( _text );
	} else {
		if(event->key()==Qt::Key_Return) {
			m_label->setText( m_label->text().append('\n') );
		} else {
			if(!event->text().isEmpty())
				m_label->setText( m_label->text() + event->text() );
		}
	}
	QWidget::keyPressEvent( event );
}

void CTextField::keyReleaseEvent( QKeyEvent * event ) {
	if(event->key()==Qt::Key_Return) {
		
	}
	QWidget::keyReleaseEvent( event );
}

void CTextField::onTextChange( const QString& _str ) {
	if(hasBackground()) {
		resize( m_label->size() );
		m_label->move( m_background->vector(5) );
		m_background->resize( width(), height() );
	} else {
		resize( m_label->size() );
		setMask( m_label->mask() );
	}
}