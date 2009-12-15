#ifndef CTEXTFIELD_H_
#define CTEXTFIELD_H_

#include <Qt/qgl.h>
#include <Qt/QList.h>

namespace gui {
	class CBorderGump;
	class CLabel;
	class CTextField : public QGLWidget
	{
		Q_OBJECT
		private:
			QList<QString> m_history;
			CLabel* m_label;
			quint32 m_stdHue;
			quint32 m_focusHue;
			CLabel* m_cursor;
			QPoint m_cursorPos;
			QVector<QString> m_text;
		public:
			CTextField(QWidget* _parent, qint32 _x, qint32 _y, QString& _text, const QString& _name = "", quint32 _focusHue = 0x34  );
			virtual ~CTextField();
			CLabel* getLabel();
			quint32 focusHue() const;
			void setFocusHue( quint32 _focusHue );
		protected:
			void focusInEvent( QFocusEvent * event );
			void focusOutEvent( QFocusEvent * event );
			void keyPressEvent( QKeyEvent * event );
			void keyReleaseEvent( QKeyEvent * event );
		public slots:
			void onTextChange(const QString& _str);
		signals:
	};
}
#endif