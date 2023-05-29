#include "transaction.h"
#include "ui_transaction.h"

Transaction::Transaction(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Transaction)
{
    ui->setupUi(this);

    // Set the display format to show only the month
    ui->dateEditMonth->setDisplayFormat("MMMM yyyy");
}

Transaction::~Transaction()
{
    delete ui;
}

void Transaction::on_btnTransaction_clicked()
{
    // Retrieve data based on the selected month
    QDate selectedDate = ui->dateEditMonth->date();
    int selectedMonth = selectedDate.month();
    int selectedYear = selectedDate.year();

    // Modify the SQL query to filter data for the selected month
    QSqlQuery query(MyDB::getInstance()->getDBInstance());
    query.prepare("SELECT "
                  "SUM(CASE WHEN SUBSTR(DOS, 6, 2) = :selectedMonth THEN SellingPrice ELSE 0 END), "
                  "SUM(CASE WHEN SUBSTR(DOP, 6, 2) = :selectedMonth THEN PurchasePrice ELSE 0 END) "
                  "FROM stocks WHERE SUBSTR(DOS, 1, 4) = :selectedYear OR SUBSTR(DOP, 1, 4) = :selectedYear");
    query.bindValue(":selectedMonth", QString("%1").arg(selectedMonth, 2, 10, QChar('0')));
    query.bindValue(":selectedYear", QString::number(selectedYear));

    if (!query.exec())
    {
        qDebug() << "Query error:" << query.lastError().text();
    }
    else
    {
        if (query.next())
        {
            double sellingPrice = query.value(0).toDouble();
            double purchasePrice = query.value(1).toDouble();

            ui->labelSellingPrice->setText(QString::number(sellingPrice));
            ui->labelPurchasePrice->setText(QString::number(purchasePrice));
        }
        else
        {
            qDebug() << "No results found.";
        }
    }

    // Close the database connection
    MyDB::getInstance()->ResetInstance();
}
