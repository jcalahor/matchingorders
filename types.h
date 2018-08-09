#include <list>
#include <vector>
#include <map>
#include "order.h"
#pragma once
typedef std::list<std::string> results_t;
typedef std::map<double, std::list<Order*>*> PriceBucket;
typedef std::map<std::string, PriceBucket*> BucketsBySymbol;
typedef std::map<double, Order*> OrdersByOID;

