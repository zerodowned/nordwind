#ifndef LOGIN_HPP
#define LOGIN_HPP

#include <QWidget>

namespace Ui {
    class Login;
}

namespace gui {
	class Login : public QWidget
	{
		Q_OBJECT;
	public:
		explicit Login(QWidget *parent = 0);
		~Login();
	private:
		Ui::Login *ui;
	};
}
#endif // LOGIN_HPP
