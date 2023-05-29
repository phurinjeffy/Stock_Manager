#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QDialog>
#include "mydb.h"
#include <QSqlQueryModel>

namespace Ui {
class Transaction;
}

class Transaction : public QDialog
{
    Q_OBJECT

public:
    explicit Transaction(QWidget *parent = nullptr);
    ~Transaction();

private slots:
    void on_btnTransaction_clicked();

private:
    Ui::Transaction *ui;
};

#endif // TRANSACTION_H
