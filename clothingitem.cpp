#include "clothingitem.h"

ClothingItem::ClothingItem(string name_, int quantity_, string status_, string dop_, string dos_, double sellingPrice_, double purchasePrice_, string size_)
    : Item(name_, quantity_, status_, dop_, dos_, sellingPrice_, purchasePrice_), size(size_) {}

ClothingItem::~ClothingItem() {
    // Destructor implementation
}

QJsonObject ClothingItem::toJson() const{
    QJsonObject itemObject = Item::toJson();
    itemObject["size"] = QString::fromStdString(size);
    return itemObject;
}

string ClothingItem::getSize() const {
    return size;
}
