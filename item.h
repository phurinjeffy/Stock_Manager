#ifndef ITEM_H
#define ITEM_H

#include "mydb.h"

#include <iostream>
#include <string>
#include <QJsonDocument>
#include <QJsonObject>

using namespace std;

class ItemHandle;

class Item {
    friend class ItemHandle;
public:
    Item(string name_, int quantity_, string status_, string dop_, string dos_, double sellingPrice_, double purchasePrice_);
    ~Item();
    virtual QJsonObject toJson() const;
    string getName() const;
    int getQuantity() const;
    string getStatus() const;
    string getDOP() const;
    string getDOS() const;
    double getSellingPrice() const;
    double getPurchasePrice() const;
    void setQuantity(int newQuantity);

protected:
    string name;
    int quantity;
    string status;
    string dop;
    string dos;
    double sellingPrice;
    double purchasePrice;
};

#endif // ITEM_H
