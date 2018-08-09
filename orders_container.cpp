#include "orders_container.h"

void OrdersContainer::StoreOrderInPriceBucket(PriceBucket *bucket, Order* order)
{
    std::list<Order*>* orders;
    auto pos = bucket -> find(order -> GetPrice());
    if ( pos == bucket -> end())
    {
        orders = new std::list<Order*>();
        orders -> push_back(order);
        bucket -> insert(std::make_pair(order -> GetPrice(), orders));
    }
    else
    {
        pos -> second -> push_back(order);
    }
}

void OrdersContainer::ClearBucket(PriceBucket *bucket)
{
        std::list<Order*> *orders;
        auto itr = bucket -> begin();
        for (; itr != bucket -> end(); itr++)
        {
            orders = itr -> second;
            bucket -> erase(itr);

            for (auto item : *orders)
                delete item;
            
            delete orders;
        }
}


bool OrdersContainer::StoreOrder(Order* order)
{
    auto posOrder = _ordersByOID.find(order -> GetOID());
    if ( posOrder == _ordersByOID.end())
    {
        _ordersByOID.insert(std::make_pair(order -> GetOID(), order));
    }
    else
    {
        _lastErrorMsg =  std::to_string(order -> GetOID()) + " Duplicate order id";
        return false;
    }

    auto pos = _bucketsBySymbol.find(order -> GetSymbol());
    PriceBucket *priceBucket;
    if ( pos == _bucketsBySymbol.end())
    {
        priceBucket = new PriceBucket();
        _bucketsBySymbol.insert(std::make_pair(order -> GetSymbol(), priceBucket));
    }
    else
    {
        priceBucket = pos -> second;
    }
    StoreOrderInPriceBucket(priceBucket, order);
    return true;
}

void OrdersContainer::Clear()
{
    auto itr = _bucketsBySymbol.begin();
    for (; itr != _bucketsBySymbol.end(); itr++)
    {
        ClearBucket(itr -> second);
        delete itr -> second;
    }
}


bool OrdersContainer::Cancel(Order *order)
{
    auto posOrder = _ordersByOID.find(order -> GetOID());
    if ( posOrder != _ordersByOID.end())
    {
        posOrder -> second -> CancelOrder();
        _lastCanceledOrder = posOrder -> second;
        return true;
    }
    else
    {
        _lastErrorMsg =  std::to_string(order -> GetOID()) + " Cannot find order id";
        return false;
    }
}