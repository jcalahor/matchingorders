#include <string>
#include "command.h"

#pragma once

class Order
{
    long _OID;
    std::string _symbol;
    SideCode _side;
    double _price;
    int _leftQuantity;
    int _quantity;
    int _fillingQuantity;
    bool _isCanceled;

    public:
        Order(long oid, std::string symbol, SideCode side, int quantity, double price):
        _OID(oid),
        _symbol(symbol),
        _side(side),
        _quantity(quantity),
        _price(price),
        _leftQuantity(quantity),
        _isCanceled(false)
        {

        }

        long GetOID()  {return _OID;}
        std::string GetSymbol() {return _symbol; }
        int GetLeftQuantity() {return _leftQuantity;}
        int GetQuantity() {return _leftQuantity;}
        double GetPrice()  {return _price;}
        SideCode GetSide()  {return _side;}
        bool IsCanceled() {return _isCanceled;}
        int GetFillingQuantity() {return _fillingQuantity;}

        void CancelOrder();
        std::string GetSideStr();
        bool TryFill(Order *order);
        std::string RenderAsFilled();
        std::string RenderAsCanceled();
        std::string RenderAsBlotter();

};
