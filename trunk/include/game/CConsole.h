#ifndef CCONSOLE_H_
#define CCONSOLE_H_

#include <Qt/QFrame.h>

class QPlainTextEdit;
class QLabel;
class QPushButton;
class QScriptEngine;

namespace gui {
	class CConsole : public QFrame
	{
		Q_OBJECT
	private:
		QPlainTextEdit* m_text;
		QLabel* m_header;
		QPushButton* m_execute;
		QPushButton* m_close;
		QPushButton* m_evaluate;
		QScriptEngine* m_engine;
	public:
		CConsole(QWidget * parent = 0, Qt::WindowFlags f = 0);
		virtual ~CConsole();
	public slots:
		void execute();
		void evaluate();
		void toggle();
	};
}
#endif