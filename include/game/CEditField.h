#ifndef CEDITFIELD_H_
#define CEDITFIELD_H_

#include "CLabel.h"

namespace gui {
	class CEditField : public QGLWidget
	{
		Q_OBJECT
	private:
		QPoint m_pos;
	public:
		CEditField(QWidget* parent = 0, Qt::WFlags flags = 0);
		virtual ~CEditField();


	};
}
#endif