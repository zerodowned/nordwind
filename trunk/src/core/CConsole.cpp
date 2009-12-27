#include "CConsole.h"
#include <Qt/QLabel.h>
#include <Qt/QPlainTextEdit.h>
#include <Qt/QPushButton.h>
#include <Qt/QScriptEngine.h>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <Qt/QDebug.h>

using namespace gui;

CConsole::CConsole( QWidget * parent, Qt::WindowFlags f ) : QFrame( parent, f ) {
	m_header = new QLabel( tr("Code:"), this );
	Q_CHECK_PTR(m_header);
	m_text = new QPlainTextEdit( this );
	m_header->setBuddy( m_text );

	m_execute = new QPushButton( tr("Execute"), this );
	m_close = new QPushButton( tr("Hide"), this );
	m_evaluate = new QPushButton( tr("Evaluate"), this );

	m_engine = new QScriptEngine();

   QVBoxLayout *leftLayout = new QVBoxLayout();
   leftLayout->addWidget(m_header);
   leftLayout->addWidget(m_text);

   QVBoxLayout *rightLayout = new QVBoxLayout();
   rightLayout->addWidget(m_execute);
   rightLayout->addWidget(m_evaluate);
   rightLayout->addWidget(m_close);
   rightLayout->addStretch();

   QHBoxLayout *mainLayout = new QHBoxLayout(this);
   mainLayout->addLayout(leftLayout);
   mainLayout->addLayout(rightLayout);
   setLayout(mainLayout);
   setWindowTitle(tr("Console"));
   //setFixedHeight(sizeHint().height());

   connect(m_close, SIGNAL(clicked()), this, SLOT(toggle()) );
   connect(m_execute, SIGNAL(clicked()), this, SLOT(execute()) );
   connect(m_evaluate, SIGNAL(clicked()), this, SLOT(evaluate()) );
}


CConsole::~CConsole() {
}

void CConsole::execute() {
	QScriptValue result = m_engine->evaluate( m_text->toPlainText() );
	qDebug() << result.toString();
}

void CConsole::evaluate() {
	QScriptSyntaxCheckResult result = m_engine->checkSyntax( m_text->toPlainText() );
	switch (result.state()) {
		case QScriptSyntaxCheckResult::Valid:
			qDebug() << "Valid Syntax";
			break;
		case QScriptSyntaxCheckResult::Error:
		case QScriptSyntaxCheckResult::Intermediate:
			qDebug() << "Invalid Syntax at" << result.errorLineNumber() << result.errorMessage();
			return;
			break;
	};
	if(m_engine->canEvaluate( m_text->toPlainText() )) {
		qDebug() << "Evaluatable";
	} else {
		qDebug() << "Non-evaluatable";
	}
}

void CConsole::toggle() {
	if(isVisible()) {
		setVisible(false);
	} else {
		setVisible(true);
	}
}
