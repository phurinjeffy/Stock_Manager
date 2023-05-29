#include "item.h"

Item:: Item(std::string name_, int quantity_, std::string status_, std::string dop_, std::string dos_, double sellingPrice_, double purchasePrice_) :
        name(name_), quantity(quantity_), status(status_), dop(dop_), dos(dos_), sellingPrice(sellingPrice_), purchasePrice(purchasePrice_) {}

Item::~Item() {
//
}

QJsonObject Item:: toJson() const {
    QJsonObject itemObject;
    itemObject["name"] = QString::fromStdString(name);
    itemObject["quantity"] = quantity;
    itemObject["status"] = QString::fromStdString(status);
    itemObject["dop"] = QString::fromStdString(dop);
    itemObject["dos"] = QString::fromStdString(dos);
    itemObject["sellingPrice"] = sellingPrice;
    itemObject["purchasePrice"] = purchasePrice;
    return itemObject;
}

string Item::getName() const{
    return name;
}

int Item::getQuantity() const{
    return quantity;
}

string Item::getStatus() const{
    return status;
}

string Item::getDOP() const{
    return dop;
}

string Item::getDOS() const{
    return dos;
}

double Item::getSellingPrice() const{
    return sellingPrice;
}

double Item::getPurchasePrice() const{
    return purchasePrice;
}

void Item::setQuantity(int newQuantity){
    this->quantity = newQuantity;
}
