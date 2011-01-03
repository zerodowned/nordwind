#ifndef NORDWIND_HPP
#define NORDWIND_HPP

#include <QApplication>
#include <QSplashScreen>

class Nordwind : public QApplication
{
    Q_OBJECT;
public:
    explicit Nordwind(int& argc, char** argv);

signals:

public slots:
        bool initialize();
        void onError(const QString& error, QString name);
private:
        QSplashScreen mSplashscreen;
};

#endif // NORDWIND_HPP
