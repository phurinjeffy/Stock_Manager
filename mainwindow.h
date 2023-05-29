#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "additem.h"
#include "itemsreport.h"
#include "expireditems.h"
#include "solditems.h"
#include "reserveditems.h"
#include "returneditems.h"
#include "availableitems.h"
#include "finditem.h"
#include "updateitem.h"
#include "deleteitem.h"
#include "transaction.h"
#include "graph.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnAddItem_clicked();
    void on_btnUpdateItem_clicked();
    void on_btnSoldItems_clicked();
    void on_btnReport_clicked();
    void on_btnExpiredItems_clicked();
    void on_btnAvailableItems_clicked();
    void on_btnReservedItem_clicked();
    void on_btnReturnedItems_clicked();
    void on_btnFindItem_clicked();
    void on_btnDeleteItem_clicked();
    void on_btnTransaction_clicked();
    void on_btnGraph_clicked();

private:
    Ui::MainWindow *ui;
    AddItem *ptrAddItem;
    ItemsReport *ptrItemReport;
    ReservedItems *ptrReservedItems;
    SoldItems *ptrSoldItems;
    ExpiredItems *ptrExpiredItems;
    ReturnedItems *ptrReturnedItems;
    AvailableItems *ptrAvailableItems;
    FindItem *ptrFindItem;
    UpdateItem *ptrUpdateItem;
    DeleteItem *ptrDeleteItem;
    Transaction *ptrTransaction;
    Graph *ptrGraph;
};

#endif // MAINWINDOW_H
