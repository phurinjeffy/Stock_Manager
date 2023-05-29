#include "reserveditems.h"
#include "ui_reserveditems.h"

ReservedItems::ReservedItems(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReservedItems)
{
    ui->setupUi(this);
}

ReservedItems::~ReservedItems()
{
    delete ui;
}

void ReservedItems::Show()
{
    updateUI();
    this->show();
}

void ReservedItems::updateUI()
{
    QSqlQueryModel * model = new QSqlQueryModel(this);

    QSqlQuery query( MyDB::getInstance()->getDBInstance());
    query.prepare("select * from stocks where Status='Reserved'");

    if(!query.exec())
       qDebug() << query.lastError().text() << query.lastQuery();
    else
       qDebug() << "== success query fetch()";

    while(query.next())
    qDebug()<<query.value(0).toString();

    model->setQuery(query);
    ui->tableView->setModel(model);
    qDebug() << "rows are : " << model->rowCount();
    ui->tableView->show();

}
