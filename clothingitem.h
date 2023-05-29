#ifndef CLOTHINGITEM_H
#define CLOTHINGITEM_H

#include "item.h"
#include "mydb.h"

#include <iostream>
#include <string>
#include <QJsonDocument>
#include <QJsonObject>

class ClothingItem : public Item {
public:
    ClothingItem(string name_, int quantity_, string status_, string dop_, string dos_, double sellingPrice_, double purchasePrice_, string size_);
    ~ClothingItem();
    QJsonObject toJson() const override;
    string getSize() const;

private:
    string size;
};
#endif // CLOTHINGITEM_H
