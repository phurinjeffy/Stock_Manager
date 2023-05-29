#ifndef ELECTRONICSITEM_H
#define ELECTRONICSITEM_H

#include "item.h"
#include "mydb.h"

#include <iostream>
#include <string>
#include <QJsonDocument>
#include <QJsonObject>

class ElectronicsItem : public Item {
public:
    ElectronicsItem(string name_, int quantity_, string status_, string dop_, string dos_, double sellingPrice_, double purchasePrice_, string brand_);
    ~ElectronicsItem();
    QJsonObject toJson() const override;
    string getBrand() const;

private:
    string brand;
};

#endif // ELECTRONICSITEM_H
