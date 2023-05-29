#ifndef ITEMHANDLE_H
#define ITEMHANDLE_H

#include "item.h"

class ItemHandle
{
public:
    ItemHandle();
    ItemHandle(Item* item);
    ~ItemHandle();

    Item* getItem() const;
    void setItem(Item* item);

private:
    Item* ptr;
};

#endif // ITEMHANDLE_H
