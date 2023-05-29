#include "electronicsitem.h"

ElectronicsItem::ElectronicsItem(string name_, int quantity_, string status_, string dop_, string dos_, double sellingPrice_, double purchasePrice_, string brand_)
    : Item(name_, quantity_, status_, dop_, dos_, sellingPrice_, purchasePrice_), brand(brand_) {}

ElectronicsItem::~ElectronicsItem() {
    // Destructor implementation
}

QJsonObject ElectronicsItem::toJson() const {
    QJsonObject itemObject = Item::toJson();
    itemObject["brand"] = QString::fromStdString(brand);
    return itemObject;
}

string ElectronicsItem::getBrand() const {
    return brand;
}
