#include "CCheckBox.h"

using namespace gui;

CCheckBox::CCheckBox(QWidget* _parent, qint32 _x, qint32 _y, const QString& _name) 
: CPushButton(_parent, _x, _y, _name), m_checked(false) {
	//buildConnections();
	connect( this, SIGNAL(checkChange(bool)), this, SLOT(onCheckChange(bool)) );
}

CCheckBox::~CCheckBox(void) {
}

inline bool CCheckBox::isChecked() const{
	return m_checked;
};

void CCheckBox::setCheck( bool _check ) {
	m_checked = _check;
	emit checkChange(m_checked);
};

void CCheckBox::onLeave() {
	if(isChecked()) {
		setState(eBtnChecked);
	} else {
		setState(eBtnStandart);
	}
}

void CCheckBox::onHovered() {
	if(isChecked()) {
		setState(eBtnCheckedHovered);
	} else {
		setState(eBtnHovered);
	}
}

void CCheckBox::onPressed() {
	if(isChecked()) {
		setState(eBtnCheckedPressed);
	} else {
		setState(eBtnPressed);
	}
}

void CCheckBox::onReleased() {
	if(isChecked()) {
		setState(eBtnCheckedHovered);
	} else {
		setState(eBtnHovered);
	}
}

void CCheckBox::onClick() {
	if(isChecked()) {
		setCheck(false);
		setState(eBtnHovered);
	} else {
		setCheck(true);
		setState(eBtnCheckedHovered);
	}
}

void CCheckBox::onCheckChange( bool _isChecked ) {
}