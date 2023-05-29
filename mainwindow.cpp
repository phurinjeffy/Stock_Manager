#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ptrAddItem = new AddItem(this);
    ptrItemReport = new ItemsReport(this);
    ptrReservedItems = new ReservedItems(this);
    ptrSoldItems = new SoldItems(this);
    ptrExpiredItems = new ExpiredItems(this);
    ptrReturnedItems = new ReturnedItems(this);
    ptrAvailableItems = new AvailableItems(this);
    ptrFindItem = new FindItem(this);
    ptrUpdateItem = new UpdateItem(this);
    ptrDeleteItem = new DeleteItem(this);
    ptrTransaction = new Transaction(this);
    ptrGraph = new Graph(this);

    this->setFixedSize(800, 600);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete ptrAddItem;
    delete ptrItemReport;
    delete ptrReservedItems;
    delete ptrSoldItems;
    delete ptrExpiredItems;
    delete ptrReturnedItems;
    delete ptrAvailableItems;
    delete ptrFindItem;
    delete ptrUpdateItem;
    delete ptrDeleteItem;
    delete ptrTransaction;
    delete ptrGraph;

    MyDB::ResetInstance();
}

void MainWindow::on_btnAddItem_clicked()
{
    ptrAddItem->show();
}

void MainWindow::on_btnUpdateItem_clicked()
{
    ptrUpdateItem->show();
}

void MainWindow::on_btnSoldItems_clicked()
{
    ptrSoldItems->Show();
}

void MainWindow::on_btnReport_clicked()
{
    ptrItemReport->Show();
}

void MainWindow::on_btnExpiredItems_clicked()
{
    ptrExpiredItems->Show();
}

void MainWindow::on_btnAvailableItems_clicked()
{
    ptrAvailableItems->Show();
}

void MainWindow::on_btnReservedItem_clicked()
{
    ptrReservedItems->Show();
}

void MainWindow::on_btnReturnedItems_clicked()
{
    ptrReturnedItems->Show();
}

void MainWindow::on_btnFindItem_clicked()
{
    ptrFindItem->show();
}

void MainWindow::on_btnDeleteItem_clicked()
{
    ptrDeleteItem->show();
}

void MainWindow::on_btnTransaction_clicked()
{
    ptrTransaction->show();
}

void MainWindow::on_btnGraph_clicked()
{
    ptrGraph->show();
}
