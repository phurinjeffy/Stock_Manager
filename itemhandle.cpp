#include "itemhandle.h"

ItemHandle::ItemHandle()
    : ptr(nullptr)
{

}

ItemHandle::ItemHandle(Item* item)
    : ptr(item)
{

}

ItemHandle::~ItemHandle()
{
    delete ptr;
}

Item* ItemHandle::getItem() const
{
    return ptr;
}

void ItemHandle::setItem(Item* item)
{
    delete ptr;
    ptr = item;
}
