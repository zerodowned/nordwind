#include "CPosition.h"

using namespace world;

CPosition::CPosition() : m_x(0), m_y(0), m_z(0), m_layer(0) {
}

CPosition::CPosition(quint32 _x, quint32 _y, qint16 _z, quint16 _layer)
: m_x(_x), m_y(_y), m_z(_z), m_layer(_layer) {
}

CPosition::CPosition(quint16 _x, quint16 _y, qint8 _z, quint16 _layer) 
: m_x(reinterpret_cast<quint32>(_x)), m_y(reinterpret_cast<quint32>(_y)), m_z(_z), m_layer(_layer) {
}

CPosition::CPosition(const world::CPosition &_r) 
: m_x(_r.x()), m_y(_r.y()), m_z(_r.z()), m_layer(_r.layer()) {
}

CPosition& CPosition::move(qint32 _x, qint32 _y, qint32 _z ) {
	m_x += x;
	m_y += _y;
	m_z += _z;
	
	return *this;
}

CPosition& CPosition::setX(quint32 _x) {
	m_x = _x;
	return *this;
}

CPosition& CPosition::setX(quint16 _x) {
	m_x = _x;
	return *this;
}

CPosition& CPosition::setY(quint32 _y) {
	m_y = _y;
	return *this;
}

CPosition& CPosition::setY(quint16 _y) {
	m_y = _y;
	return *this;
}

CPosition& CPosition::setZ(qint32 _z) {
	m_z = _z;
	return *this;
}

CPosition& CPosition::setZ(qint8 _z) {
	m_z = _z;
	return *this;
}

CPosition& CPosition::setLayer( quint15 _layer ) {
	m_layer = _layer;
}

quint32& CPosition::x() const {
	return m_x;
}

quint32& CPosition::y() const {
	return m_y;
}

qint32& CPosition::z() const {
	return m_z;
}

quint16& CPosition::layer() const {
	return m_layer;
}

quint32 CPosition::blockId() const {
	return (m_x
}