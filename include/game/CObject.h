#ifndef COBJECT_H__
#define COBJECT_H__
#include <CPosition.h>

namespace world {
	class CObject {
		private:
			CPosition m_pos;
			quint32 m_type;
			quint32 m_tooltip;
			// reference to over-head text
		public:
			CObject( quint32 _type, quint32 x, quint32 y, qint16 z ) 
				: m_type(_type), m_pos(CPosition(x,y,z)) {
			}
			CObject( quint32 _type, CPosition& _pos ) 
				: m_type(_type), m_pos(_pos) {
			};
			CPosition& pos() {
				return &m_pos;
			};
			void setPos( CPosition& _newpos ) {
				m_pos = _newpos;
			};
			quint32 type() {
				return m_type;
			};
			void setType( quint32& _type ) {
				m_type = _type;
			};
	}
}
#endif