#include "furnitureitem.h"

FurnitureItem::FurnitureItem(string name_, int quantity_, string status_, string dop_, string dos_, double sellingPrice_, double purchasePrice_, string material_)
    : Item(name_, quantity_, status_, dop_, dos_, sellingPrice_, purchasePrice_), material(material_) {}

FurnitureItem::~FurnitureItem() {
    // Destructor implementation
}

QJsonObject FurnitureItem::toJson() const {
    QJsonObject itemObject = Item::toJson();
    itemObject["material"] = QString::fromStdString(material);
    return itemObject;
}

string FurnitureItem::getMaterial() const {
    return material;
}
