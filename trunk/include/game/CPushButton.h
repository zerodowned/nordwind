#ifndef CPUSHBUTTON_H
#define CPUSHBUTTON_H

#include <Qt/qgl.h>
#include <Qt/QMap.h>

namespace resource {
    class CGump;
}

namespace gui {
	enum eButtonState {
		eBtnStandart,
		eBtnPressed,
		eBtnClicked,
		eBtnChecked,
		eBtnCheckedHovered,
		eBtnCheckedPressed,
		eBtnCheckedClicked,
		eBtnReleased,
		eBtnHovered,
		eBtnFocused
	};

    class CPushButton : public QGLWidget
    {
		Q_OBJECT;
        private:
			QMap<eButtonState, resource::CGump*> m_stateGumps;
			eButtonState m_state;
			bool m_isPressed;
			resource::CGump* getCurrent() const;
        protected:
            void paintEvent ( QPaintEvent * event );
			void leaveEvent ( QEvent * event );
			void enterEvent ( QEvent * event );
			void mousePressEvent ( QMouseEvent * event );
			void mouseReleaseEvent ( QMouseEvent * event );
			void buildConnections( void );
        public:
			CPushButton( QWidget* _parent, qint32 _x, qint32 _y, const QString& _name = "" );
			virtual ~CPushButton();
			CPushButton& addState( eButtonState _state, quint32 _id, quint32 _hue = 0, bool partialHue = false );
			void setState( eButtonState _newState );
			eButtonState isState() const;
		public slots:
			void onStateChanged();
			virtual void onLeave();
			virtual void onHovered();
			virtual void onPressed();
			virtual void onReleased();
			virtual void onClick();
		signals:
			void stateChanged();
			void leave();
			void hovered();
			void pressed();
			void released();
			void click();
    };
}

#endif // CPUSHBUTTON_H
