#include "CLabel.h"
#include "../resource/CAsciiFonts.h"
#include "../CClient.h"
#include <Qt/QBitmap.h>
#include <Qt/QPainter.h>

using namespace gui;

CLabel::CLabel(QWidget* parent, qint32 _x, qint32 _y, QString& _text, const QString& _name) 
: QGLWidget(parent, Client::instance().gui()), m_image(NULL), m_text(_text), m_font(resource::eFont_Ascii), m_fontid(0), m_hue(0) {
	if(_name!="")
		setObjectName(_name);
	move(_x,_y);
	connect(this, SIGNAL(textChange(const QString&)), this, SLOT(onTextChange(const QString&)) );
	connect(this, SIGNAL(hueChange( quint32) ), this, SLOT(onHueChange(quint32)) );
	connect(this, SIGNAL(fontChange(resource::eFontTypes, quint8)), this, SLOT(onFontChange(resource::eFontTypes, quint8)) );
	syncImage();
}


CLabel::~CLabel() {
	if(m_image)
		delete m_image;
}

void CLabel::syncImage() {
	QImage* img = resource::CAsciiEngine::instance().buildText( m_fontid, m_text, m_hue );
	if(img) {
		if(m_image)
			delete m_image;
		m_image = img;
		setMask(QBitmap::fromImage( m_image->createAlphaMask() ) );
		resize( m_image->width(), m_image->height() );
	}
	update();
}

QImage& CLabel::image() const {
	return *m_image;
}

void CLabel::setText(const QString &_str) {
	m_text = _str;
	emit textChange( _str );
}

QString CLabel::text() const {
	return m_text;
}

void CLabel::setHue(quint32 _hue) {
	m_hue = _hue;
	emit hueChange( _hue );
}

quint32 CLabel::hue() const {
	return m_hue;
}

void CLabel::setFont(resource::eFontTypes _font, quint8 _id) {
	m_font = _font;
	m_fontid = _id;
	emit fontChange( _font, _id );
}

quint8 CLabel::fontId() const {
	return m_fontid;
}

resource::eFontTypes CLabel::fontType() const {
	return m_font;
}

void CLabel::paintEvent(QPaintEvent *event) {
	Q_UNUSED(event);
	QPainter painter(this);
	if(m_image) {
		painter.drawImage(QPoint(0,0), image(), image().rect() );
	} else {
		painter.drawText(QPoint(0,0), text() );
	}
	painter.end();
}

void CLabel::onTextChange( const QString& _str ) {
	syncImage();
}

void CLabel::onHueChange(quint32 _hue) {
	syncImage();
}

void CLabel::onFontChange(resource::eFontTypes _font, quint8 _id) {
	syncImage();
}