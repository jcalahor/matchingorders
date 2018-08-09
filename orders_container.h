#include "types.h"
#include "order.h"
#include <string>
#pragma once

class OrdersContainer
{
    BucketsBySymbol _bucketsBySymbol;
    OrdersByOID _ordersByOID;
    Order * _lastCanceledOrder;
    std::string _lastErrorMsg;

    void ClearBucket(PriceBucket *bucket);
    void StoreOrderInPriceBucket(PriceBucket *bucket, Order* order);
    public:
        bool StoreOrder(Order* order);
        void Clear();
        bool Cancel(Order *order);

        BucketsBySymbol& GetBucketsBySymbolObj() {return _bucketsBySymbol;}
        Order *  GetLastCanceleOrder(){ return _lastCanceledOrder;}
        std::string& GetLastErrorMsg() { return _lastErrorMsg;}
};
