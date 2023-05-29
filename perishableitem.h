#ifndef PERISHABLEITEM_H
#define PERISHABLEITEM_H

#include "item.h"
#include "mydb.h"

#include <iostream>
#include <string>
#include <QJsonDocument>
#include <QJsonObject>

class PerishableItem : public Item {
public:
    PerishableItem(string name_, int quantity_, string status_, string dop_, string dos_, double sellingPrice_, double purchasePrice_, string foodType_);
    ~PerishableItem();
    QJsonObject toJson() const override;
    string getFoodType() const;

private:
    string foodType;
};

#endif // PERISHABLEITEM_H
