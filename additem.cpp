#include "additem.h"
#include "ui_additem.h"

#include "item.h"
#include "clothingitem.h"
#include "electronicsitem.h"
#include "perishableitem.h"
#include "furnitureitem.h"

#include "mydb.h"

#include <QDate>
#include <QJsonDocument>
#include <QJsonObject>

#include <QFileDialog>
#include <QTextStream>

AddItem::AddItem(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddItem)
{
    ui->setupUi(this);
}

AddItem::~AddItem()
{
    delete ui;
}

void AddItem::Show()
{
    this->show();
}

std::unique_ptr<Item> AddItem::createObject(string type, string name, int quantity, string status, string dop, string dos, double sellingPrice, double purchasePrice, string details)
{
    std::unique_ptr<Item> newItem = nullptr;

    if (type == "Clothing") {
        newItem = std::make_unique<ClothingItem>(name, quantity, status, dop, dos, sellingPrice, purchasePrice, details);
    }
    else if (type == "Electronics") {
        newItem = std::make_unique<ElectronicsItem>(name, quantity, status, dop, dos, sellingPrice, purchasePrice, details);
    }
    else if (type == "Perishable") {
        newItem = std::make_unique<PerishableItem>(name, quantity, status, dop, dos, sellingPrice, purchasePrice, details);
    }
    else if (type == "Furniture") {
        newItem = std::make_unique<FurnitureItem>(name, quantity, status, dop, dos, sellingPrice, purchasePrice, details);
    }

    return newItem;
}

QByteArray AddItem::convertToJson(const Item& item)
{
    // Convert derived class object to a JSON object
    QJsonObject itemObject = item.toJson();

    // Convert the JSON object to a JSON document
    QJsonDocument jsonDoc(itemObject);
    QByteArray jsonStr = jsonDoc.toJson(QJsonDocument::Indented);

    return jsonStr;
}

std::unique_ptr<Item> AddItem::deserializeFromJson(const QByteArray& jsonStr)
{
    // Deserialize the JSON string to a QJsonObject
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonStr);
    QJsonObject jsonObj = jsonDoc.object();

    // Extract the relevant values from the JSON object
    std::string type = jsonObj["Type"].toString().toStdString();
    std::string name = jsonObj["Name"].toString().toStdString();
    int quantity = jsonObj["Quantity"].toInt();
    std::string status = jsonObj["Status"].toString().toStdString();
    std::string dop = jsonObj["DOP"].toString().toStdString();
    std::string dos = jsonObj["DOS"].toString().toStdString();
    double sellingPrice = jsonObj["SellingPrice"].toDouble();
    double purchasePrice = jsonObj["PurchasePrice"].toDouble();

    std::string details;

    if (type == "Clothing") {
        details = jsonObj["size"].toString().toStdString();
    } else if (type == "Electronics") {
        details = jsonObj["brand"].toString().toStdString();
    } else if (type == "Perishable") {
        details = jsonObj["foodType"].toString().toStdString();
    } else if (type == "Furniture") {
        details = jsonObj["material"].toString().toStdString();
    }

    // Create an instance of the derived class based on the 'type'
    std::unique_ptr<Item> item = createObject(type, name, quantity, status, dop, dos, sellingPrice, purchasePrice, details);

    return item;
}

void AddItem::on_btnAdd_clicked()
{
    ui->lblInfo->setText("");
    QString sName = ui->txtName->text();
    QString sQuantityStr = ui->txtQuantity->text();

    // Validate input for Quantity
    bool ok;
    int sQuantity = sQuantityStr.toInt(&ok);
    if (!ok)
    {
        // Invalid input for Quantity
        qDebug() << "Invalid input for Quantity";
        ui->lblInfo->setText("Invalid input for Quantity");
        return;
    }

    QString sStatus = ui->cmbStatus->currentText();
    QString sType = ui->cmbType->currentText();
    QString sDetails = ui->txtDetails->text();

    QDate sDOP;
    if (!ui->cmbDOPDay->currentText().isEmpty() && !ui->cmbDOPMonth->currentText().isEmpty() && !ui->cmbDOPYear->currentText().isEmpty())
    {
        sDOP = QDate(ui->cmbDOPYear->currentText().toInt(),
                     ui->cmbDOPMonth->currentIndex(),
                     ui->cmbDOPDay->currentIndex());
    }

    QDate sDOS;
    if (!ui->cmbDOSDay->currentText().isEmpty() && !ui->cmbDOSMonth->currentText().isEmpty() && !ui->cmbDOSYear->currentText().isEmpty())
    {
        sDOS = QDate(ui->cmbDOSYear->currentText().toInt(),
                     ui->cmbDOSMonth->currentIndex(),
                     ui->cmbDOSDay->currentIndex());

        if (sDOS < sDOP)
        {
            sDOS = QDate(0, 0, 0);
        }
    }

    QString sSellingPriceStr = ui->txtSellingPrice->text();
    double sSellingPrice = sSellingPriceStr.isEmpty() ? QVariant::Invalid : sSellingPriceStr.toDouble(&ok);

    QString sPurchasePriceStr = ui->txtPurchasePrice->text();
    double sPurchasePrice = sPurchasePriceStr.isEmpty() ? QVariant::Invalid : sPurchasePriceStr.toDouble(&ok);

    // Check if item with the same name already exists
    QSqlQuery selectQuery(MyDB::getInstance()->getDBInstance());
    selectQuery.prepare("SELECT * FROM stocks WHERE Name = :name");
    selectQuery.bindValue(":name", sName);

    if (!selectQuery.exec()) {
        qDebug() << "Error occurred while checking if item exists:" << selectQuery.lastError().text();
        return;
    }

    if (selectQuery.next()) {
        // Item already exists, update the quantity
        int currentQuantity = selectQuery.value("Quantity").toInt();
        int newQuantity = currentQuantity + sQuantity;

        // Create a pointer to the derived class type
        std::unique_ptr<Item> newItem = createObject(sType.toStdString(), sName.toStdString(), newQuantity, sStatus.toStdString(), sDOP.toString(Qt::ISODate).toStdString(), sDOS.toString(Qt::ISODate).toStdString(), sSellingPrice, sPurchasePrice, sDetails.toStdString());

        // Convert derived class object to a JSON object to JSON Document
        QByteArray jsonStr = convertToJson(*newItem);

        QSqlQuery updateQuery(MyDB::getInstance()->getDBInstance());
        updateQuery.prepare("UPDATE stocks SET Quantity = :quantity, ItemData = :itemData WHERE Name = :name");
        updateQuery.bindValue(":quantity", newItem->getQuantity());
        updateQuery.bindValue(":itemData", jsonStr);
        updateQuery.bindValue(":name", QString::fromStdString(newItem->getName()));

        if (!updateQuery.exec()) {
            qDebug() << "Error occurred while updating item quantity:" << updateQuery.lastError().text();
            ui->lblInfo->setText("Unable to update item quantity");
            return;
        }

        ui->lblInfo->setText("Item quantity updated successfully!");
        resetElements();

    } else {
        // Item doesn't exist, insert a new item

        // Create a pointer to the derived class type
        std::unique_ptr<Item> newItem = createObject(sType.toStdString(), sName.toStdString(), sQuantity, sStatus.toStdString(), sDOP.toString(Qt::ISODate).toStdString(), sDOS.toString(Qt::ISODate).toStdString(), sSellingPrice, sPurchasePrice, sDetails.toStdString());

        // Convert derived class object to a JSON object to JSON Document
        QByteArray jsonStr = convertToJson(*newItem);

        QSqlQuery insertQuery(MyDB::getInstance()->getDBInstance());
        insertQuery.prepare("INSERT INTO stocks (Name, Quantity, Status, DOP, DOS, SellingPrice, PurchasePrice, Details, ItemData) "
                            "VALUES (:name, :quantity, :status, :dop, :dos, :sellingPrice, :purchasePrice, :details, :itemData)");

        insertQuery.bindValue(":name", QString::fromStdString(newItem->getName()));
        insertQuery.bindValue(":quantity", newItem->getQuantity());
        insertQuery.bindValue(":status", QString::fromStdString(newItem->getStatus()));
        if (sDOP.isValid())
        {
            insertQuery.bindValue(":dop", QString::fromStdString(newItem->getDOP()));
        }
        else
        {
            insertQuery.bindValue(":dop", QVariant::Invalid);
        }
        if (sDOS.isValid())
        {
            insertQuery.bindValue(":dos", QString::fromStdString(newItem->getDOS()));
        }
        else
        {
            insertQuery.bindValue(":dos", QVariant::Invalid);
        }
        insertQuery.bindValue(":sellingPrice", newItem->getSellingPrice());
        insertQuery.bindValue(":purchasePrice", newItem->getPurchasePrice());
        insertQuery.bindValue(":details", sDetails);
        insertQuery.bindValue(":itemData", jsonStr);

        if (!insertQuery.exec())
        {
            qDebug() << insertQuery.lastError().text() << insertQuery.lastQuery();
            ui->lblInfo->setText("Unable to Add Item");
        }
        else
        {
            if (insertQuery.numRowsAffected() > 0)
            {
                qDebug() << "Insert was successful" << insertQuery.lastQuery();
                ui->lblInfo->setText("Item Added Successfully!");
                resetElements();
            }
            else
                qDebug() << "Unable to Add new Item";
        }
    }
}


void AddItem::on_btnChooseFile_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select File"), "", tr("Text Files (*.txt)"));
    if (!filePath.isEmpty())
    {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            while (!in.atEnd())
            {
                QString line = in.readLine();
                processFileLine(line);
            }
            file.close();
        }
    }
}

void AddItem::processFileLine(const QString& line)
{
    QStringList parts = line.split(",");
    if (parts.size() == 9)
    {
        QString sType = parts[0].trimmed();
        QString sName = parts[1].trimmed();
        QString sQuantityStr = parts[2].trimmed();
        QString sStatus = parts[3].trimmed();
        QString sDOPStr = parts[4].trimmed();
        QString sDOSStr = parts[5].trimmed();
        QString sSellingPriceStr = parts[6].trimmed();
        QString sPurchasePriceStr = parts[7].trimmed();
        QString sDetails = parts[8].trimmed();

        bool ok;
        int sQuantity = sQuantityStr.toInt(&ok);
        if (!ok)
        {
            qDebug() << "Invalid input for Quantity";
            return;
        }

        QDate sDOP = QDate::fromString(sDOPStr, Qt::ISODate);
        QDate sDOS = QDate::fromString(sDOSStr, Qt::ISODate);
        double sSellingPrice = sSellingPriceStr.isEmpty() ? QVariant::Invalid : sSellingPriceStr.toDouble(&ok);
        double sPurchasePrice = sPurchasePriceStr.isEmpty() ? QVariant::Invalid : sPurchasePriceStr.toDouble(&ok);

        QSqlQuery selectQuery(MyDB::getInstance()->getDBInstance());
        selectQuery.prepare("SELECT * FROM stocks WHERE Name = :name");
        selectQuery.bindValue(":name", sName);

        if (selectQuery.exec() && selectQuery.next())
        {   
            // Item already exists, update the quantity
            int currentQuantity = selectQuery.value("Quantity").toInt();
            int newQuantity = currentQuantity + sQuantity;

            // Create a pointer to the derived class type
            std::unique_ptr<Item> newItem = createObject(sType.toStdString(), sName.toStdString(), newQuantity, sStatus.toStdString(), sDOP.toString(Qt::ISODate).toStdString(), sDOS.toString(Qt::ISODate).toStdString(), sSellingPrice, sPurchasePrice, sDetails.toStdString());

            // Convert derived class object to a JSON object to JSON Document
            QByteArray jsonStr = convertToJson(*newItem);

            QSqlQuery updateQuery(MyDB::getInstance()->getDBInstance());
            updateQuery.prepare("UPDATE stocks SET Quantity = :quantity, ItemData = :itemData WHERE Name = :name");
            updateQuery.bindValue(":quantity", newItem->getQuantity());
            updateQuery.bindValue(":itemData", jsonStr);
            updateQuery.bindValue(":name", QString::fromStdString(newItem->getName()));

            if (!updateQuery.exec())
            {
                qDebug() << "Unable to update item quantity:" << updateQuery.lastError().text();
                return;
            }

            qDebug() << "Item quantity updated successfully!";
            resetElements();
        }

        else
        {
            // Item doesn't exist, insert a new item

            // Create a pointer to the derived class type
            std::unique_ptr<Item> newItem = createObject(sType.toStdString(), sName.toStdString(), sQuantity, sStatus.toStdString(), sDOP.toString(Qt::ISODate).toStdString(), sDOS.toString(Qt::ISODate).toStdString(), sSellingPrice, sPurchasePrice, sDetails.toStdString());

            // Convert derived class object to a JSON object to JSON Document
            QByteArray jsonStr = convertToJson(*newItem);

            QSqlQuery insertQuery(MyDB::getInstance()->getDBInstance());
            insertQuery.prepare("INSERT INTO stocks (Name, Quantity, Status, DOP, DOS, SellingPrice, PurchasePrice, Details, ItemData) "
                                "VALUES (:name, :quantity, :status, :dop, :dos, :sellingPrice, :purchasePrice, :details, :itemData)");

            insertQuery.bindValue(":name", QString::fromStdString(newItem->getName()));
            insertQuery.bindValue(":quantity", newItem->getQuantity());
            insertQuery.bindValue(":status", QString::fromStdString(newItem->getStatus()));
            if (sDOP.isValid())
            {
                insertQuery.bindValue(":dop", QString::fromStdString(newItem->getDOP()));
            }
            else
            {
                insertQuery.bindValue(":dop", QVariant::Invalid);
            }
            if (sDOS.isValid())
            {
                insertQuery.bindValue(":dos", QString::fromStdString(newItem->getDOS()));
            }
            else
            {
                insertQuery.bindValue(":dos", QVariant::Invalid);
            }
            insertQuery.bindValue(":sellingPrice", newItem->getSellingPrice());
            insertQuery.bindValue(":purchasePrice", newItem->getPurchasePrice());
            insertQuery.bindValue(":details", sDetails);
            insertQuery.bindValue(":itemData", jsonStr);

            if (!insertQuery.exec())
            {
                qDebug() << "Unable to add item:" << insertQuery.lastError().text();
                return;
            }

            qDebug() << "Item added successfully!";
        }
    }

    ui->lblInfo->setText("Items added successfully!");
    resetElements();
}

void AddItem::resetElements()
{
    ui->txtName->clear();
    ui->txtQuantity->clear();
    ui->txtSellingPrice->clear();
    ui->txtPurchasePrice->clear();
    ui->txtDetails->clear();
}
