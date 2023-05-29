#ifndef FURNITUREITEM_H
#define FURNITUREITEM_H

#include "item.h"
#include "mydb.h"

#include <iostream>
#include <string>
#include <QJsonDocument>
#include <QJsonObject>

class FurnitureItem : public Item {
public:
    FurnitureItem(string name_, int quantity_, string status_, string dop_, string dos_, double sellingPrice_, double purchasePrice_, string material_);
    ~FurnitureItem();
    QJsonObject toJson() const override;
    string getMaterial() const;

private:
    string material;
};

#endif // FURNITUREITEM_H
