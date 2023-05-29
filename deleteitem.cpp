#include "deleteitem.h"
#include "ui_deleteitem.h"

#include <QJsonDocument>
#include <QJsonObject>

DeleteItem::DeleteItem(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeleteItem)
{
    ui->setupUi(this);
}

DeleteItem::~DeleteItem()
{
    delete ui;
}

void DeleteItem::on_btnDeleteItem_clicked()
{
    ui->lblInfo->clear();
    QString sValue = ui->txtValue->text();

    QString sCriteriaColumn = "";
    if(ui->rdoId->isChecked())
        sCriteriaColumn="Id";
    else if (ui->rdoName->isChecked())
        sCriteriaColumn="Name";
    else if (ui->rdoQuantity->isChecked())
        sCriteriaColumn="Quantity";

    int quantityToRemove = ui->spinQuantity->value();

    // Retrieve the JSON data for the item
    QSqlQuery selectQuery(MyDB::getInstance()->getDBInstance());
    QString selectQueryString = "SELECT ItemData FROM stocks WHERE " + sCriteriaColumn + " = :value";
    selectQuery.prepare(selectQueryString);
    selectQuery.bindValue(":value", sValue);

    if (!selectQuery.exec() || !selectQuery.next()) {
        qDebug() << "Error occurred while fetching item data:" << selectQuery.lastError().text();
        return;
    }

    QString itemDataJsonStr = selectQuery.value(0).toString();

    // Parse the JSON data
    QJsonDocument jsonDoc = QJsonDocument::fromJson(itemDataJsonStr.toUtf8());
    if (!jsonDoc.isObject()) {
        qDebug() << "Invalid JSON data for the item";
        return;
    }

    QJsonObject itemObject = jsonDoc.object();

    // Update the quantity value
    int currentQuantity = itemObject["quantity"].toInt();
    if (quantityToRemove > currentQuantity) {
        qDebug() << "Error: Quantity to remove exceeds the available item quantity.";
        ui->lblInfo->setText("Quantity to remove exceeds item quantity");
        return;
    }
    int newQuantity = currentQuantity - quantityToRemove;
    itemObject["quantity"] = newQuantity;

    // Convert the updated QJsonObject back to a JSON string
    QString updatedItemDataJsonStr = QJsonDocument(itemObject).toJson(QJsonDocument::Indented);

    // Execute the update query with the updated JSON string
    QSqlQuery updateQuery(MyDB::getInstance()->getDBInstance());
    QString updateQueryString = "UPDATE stocks SET Quantity=:quantity, ItemData=:itemData WHERE " + sCriteriaColumn + " = :value";
    updateQuery.prepare(updateQueryString);
    updateQuery.bindValue(":quantity", newQuantity);
    updateQuery.bindValue(":itemData", updatedItemDataJsonStr);
    updateQuery.bindValue(":value", sValue);

    if (!updateQuery.exec()) {
        qDebug() << "Error occurred while deleting item:" << updateQuery.lastError().text();
        ui->lblInfo->setText("Unable to delete item");
    } else {
        if (updateQuery.numRowsAffected() > 0) {
            qDebug() << "Delete successfully!";
            ui->lblInfo->setText("Item deleted successfully!");
        } else {
            qDebug() << "No item found with the specified value.";
            ui->lblInfo->setText("No item found with the specified value.");
        }
    }
}

void DeleteItem::resetElements()
{
    ui->txtValue->clear();
    ui->spinQuantity->setValue(0);
}
