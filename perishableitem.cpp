#include "perishableitem.h"

PerishableItem::PerishableItem(string name_, int quantity_, string status_, string dop_, string dos_, double sellingPrice_, double purchasePrice_, string foodType_)
    : Item(name_, quantity_, status_, dop_, dos_, sellingPrice_, purchasePrice_), foodType(foodType_) {}

PerishableItem::~PerishableItem() {
    // Destructor implementation
}

QJsonObject PerishableItem::toJson() const {
    QJsonObject itemObject = Item::toJson();
    itemObject["foodType"] = QString::fromStdString(foodType);
    return itemObject;
}

string PerishableItem::getFoodType() const {
    return foodType;
}
