#include "order_filler.h"


bool OrderFiller::IsGoodPrice(Order *order, double price)
{
    if (order -> GetSide() == SideCode::Buy)
    {
        return (order -> GetPrice() >= price);
    }
    else
    {
        return (order -> GetPrice() <= price);
    }
}

bool OrderFiller::contains(std::list<Order*> * orders, const Order * element)
{
    auto it = std::find(orders -> begin(), orders -> end(), element);
    return it != orders -> end();
}


void OrderFiller::TryFillInPriceBucket(std::list<Order*>* orders,  Order *fillingOrder)
{            
    if (fillingOrder -> GetLeftQuantity() > 0)
    {
        for (auto order : *orders)
        {
            if (fillingOrder -> TryFill(order))
            {
                if (!contains(&_fillingOrders, order))
                    _fillingOrders.push_back(order);
                if (!contains(&_fillingOrders, fillingOrder))
                    _fillingOrders.push_back(fillingOrder);
            }
        }                       
    }
}

void OrderFiller::TryFillBySymbol(PriceBucket *priceBucket, Order *order)
{
    auto itr = priceBucket -> begin();
    for (; itr != priceBucket -> end(); itr++)
    {
        if (order -> GetLeftQuantity() > 0)
        {
            if (IsGoodPrice(order, itr -> first))
            {
                TryFillInPriceBucket(itr -> second, order);
            }
        }
    }
}

void OrderFiller::TryFill(Order *order)
{
    _fillingOrders.clear();
    auto pos = _ordersContainer -> GetBucketsBySymbolObj().find(order -> GetSymbol());
    PriceBucket *priceBucket;
    if ( pos != _ordersContainer -> GetBucketsBySymbolObj().end())
    {
        TryFillBySymbol(pos -> second, order);                
    }
}
