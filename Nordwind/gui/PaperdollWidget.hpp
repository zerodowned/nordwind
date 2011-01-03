#ifndef PAPERDOLL_HPP
#define PAPERDOLL_HPP

#include <QWidget>

namespace Ui {
    class Paperdoll;
}

class Paperdoll : public QWidget
{
    Q_OBJECT

public:
    explicit Paperdoll(QWidget *parent = 0);
    ~Paperdoll();

private:
    Ui::Paperdoll *ui;
};

#endif // PAPERDOLL_HPP
