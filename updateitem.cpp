#include "updateitem.h"
#include "ui_updateitem.h"

#include <QJsonDocument>
#include <QJsonObject>

UpdateItem::UpdateItem(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateItem)
{
    ui->setupUi(this);
}

UpdateItem::~UpdateItem()
{
    delete ui;
}

void UpdateItem::updateUI()
{

}

void UpdateItem::Show()
{
    updateUI();
    this->show();
}

void UpdateItem::on_pushButton_clicked()
{
    ui->lblInfo->clear();
    QString sValue = ui->txtValue->text();
    QString sStatus = ui->cmbStatus->currentText();

    QString sCriteriaColumn = "";
    if(ui->rdoId->isChecked())
        sCriteriaColumn="Id";
    else if (ui->rdoName->isChecked())
        sCriteriaColumn="Name";
    else if (ui->rdoQuantity->isChecked())
        sCriteriaColumn="Quantity";

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
    QString newStatus = ui->cmbStatus->currentText();
    itemObject["status"] = newStatus;

    // Convert the updated QJsonObject back to a JSON string
    QString updatedItemDataJsonStr = QJsonDocument(itemObject).toJson(QJsonDocument::Indented);

    // Execute the update query with the updated JSON string
    QSqlQuery updateQuery(MyDB::getInstance()->getDBInstance());
    QString updateQueryString = "UPDATE stocks SET Status=:status, ItemData=:itemData WHERE " + sCriteriaColumn + " = :value";
    updateQuery.prepare(updateQueryString);
    updateQuery.bindValue(":status", sStatus);
    updateQuery.bindValue(":itemData", updatedItemDataJsonStr);
    updateQuery.bindValue(":value", sValue);

    if (!updateQuery.exec()) {
        qDebug() << "Error occurred while updating item:" << updateQuery.lastError().text();
        ui->lblInfo->setText("Unable to update item");
    } else {
        if (updateQuery.numRowsAffected() > 0) {
            qDebug() << "Item updated successfully!";
            ui->lblInfo->setText("Item updated successfully!");
        } else {
            qDebug() << "No item found with the specified value.";
            ui->lblInfo->setText("No item found with the specified value.");
        }
    }
}
