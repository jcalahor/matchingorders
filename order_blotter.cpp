#include "order_blotter.h"

void OrderBlotter::BuildBlotterInBucket(PriceBucket *bucket)
{
    std::list<Order*> *orders;

    for (auto iter = bucket -> rbegin(); iter != bucket -> rend(); ++iter)
    {
        orders = iter -> second;

        for (auto item : *orders)
        {
            if (item -> GetLeftQuantity() > 0 && !item -> IsCanceled())
                _currentBlotter.push_back(item);
        }                
    }
}

void OrderBlotter::BuildBlotter()
{
    _currentBlotter.clear();
    auto itr = _ordersContainer -> GetBucketsBySymbolObj().begin();
    for (; itr != _ordersContainer -> GetBucketsBySymbolObj().end(); itr++)
    {
        BuildBlotterInBucket(itr -> second);
    }
}