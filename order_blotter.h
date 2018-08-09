#include "types.h"

#include "orders_container.h"

#pragma once

class OrderBlotter
{
    OrdersContainer*  _ordersContainer;
    std::list<Order*>  _currentBlotter;
    void BuildBlotterInBucket(PriceBucket *bucket);
    public:
        OrderBlotter(OrdersContainer* ordersContainer)
        {
            _ordersContainer = ordersContainer;
        }
        void BuildBlotter();

        std::list<Order*>& GetCurrentBlotter() {return _currentBlotter;}
};