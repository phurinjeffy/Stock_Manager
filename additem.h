#ifndef ADDITEM_H
#define ADDITEM_H

#include <QDialog>
#include "mydb.h"

#include "item.h"

namespace Ui {
class AddItem;
}

class AddItem : public QDialog
{
    Q_OBJECT

public:
    explicit AddItem(QWidget *parent = nullptr);
    ~AddItem();
    void Show();

private slots:
    void on_btnAdd_clicked();
    void on_btnChooseFile_clicked();
    void processFileLine(const QString& line);

private:
    Ui::AddItem *ui;
    void resetElements();
    std::unique_ptr<Item> createObject(std::string type, std::string name, int quantity, std::string status, std::string dop, std::string dos, double sellingPrice, double purchasePrice, std::string details);
    QByteArray convertToJson(const Item& item);
    std::unique_ptr<Item> deserializeFromJson(const QByteArray& jsonStr);
};

#endif // ADDITEM_H
