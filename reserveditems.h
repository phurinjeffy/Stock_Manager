#ifndef RESERVEDITEMS_H
#define RESERVEDITEMS_H

#include <QDialog>
#include "mydb.h"

namespace Ui {
class ReservedItems;
}

class ReservedItems : public QDialog
{
    Q_OBJECT

public:
    explicit ReservedItems(QWidget *parent = nullptr);
    ~ReservedItems();
     void Show();

private:
    Ui::ReservedItems *ui;
    void updateUI();
};

#endif // RESERVEDITEMS_H
