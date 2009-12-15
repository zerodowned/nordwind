#ifndef CCHECKBOX_H
#define CCHECKBOX_H

#include "CPushButton.h"

namespace gui {
	class CCheckBox : public CPushButton {
		Q_OBJECT;
		private:
			bool m_checked;
		public:
			CCheckBox(QWidget* _parent, qint32 _x, qint32 _y, const QString& _name = "");
			virtual ~CCheckBox(void);
			bool isChecked() const;
			void setCheck(bool _check);
		public slots:
			void onLeave();
			void onHovered();
			void onPressed();
			void onReleased();
			void onClick();
			void onCheckChange( bool _isChecked);
		signals:
			void checkChange(bool _isChecked);
	};
}

#endif