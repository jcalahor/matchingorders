#include "types.h"
#include "order.h"
#include "orders_container.h"
#include <list>
#include <vector>
#include <string>

#pragma once

class OrderFiller
{
    std::list<Order*> _fillingOrders;
    OrdersContainer*  _ordersContainer;
    bool IsGoodPrice(Order *order, double price);
    bool contains(std::list<Order*> * orders, const Order * element);
    void TryFillInPriceBucket(std::list<Order*>* orders,  Order *fillingOrder);
    void TryFillBySymbol(PriceBucket *priceBucket, Order *order);

    public:

    OrderFiller(OrdersContainer* ordersContainer)
    {
        _ordersContainer = ordersContainer;
    }

    void TryFill(Order *order);
    std::list<Order*> & GetFillingOrders() { return _fillingOrders; }

};