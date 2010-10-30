#ifndef PAPERDOLL_HPP
#define PAPERDOLL_HPP

#include <QWidget>

namespace Ui {
    class PaperdollWidget;
}

class PaperdollWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PaperdollWidget(QWidget *parent = 0);
    ~PaperdollWidget();

private:
    Ui::PaperdollWidget *ui;
};

#endif // PAPERDOLL_HPP
