#include "order.h"
#include <iomanip>

void Order::CancelOrder()
{
    _isCanceled = true;
}

std::string Order::GetSideStr()
{
    if (_side == SideCode::Buy)
        return "B";
    else
        return "S";

}

bool Order::TryFill(Order *order)
{
    if (IsCanceled() || order -> IsCanceled() ||  GetOID() == order -> GetOID() ||  GetSide() == order -> GetSide() ||  
        GetLeftQuantity() == 0 || order ->  GetLeftQuantity() == 0 || GetSymbol() != order -> GetSymbol())
    {
        return false;
    }

    if (order -> _leftQuantity > _leftQuantity)
    {
        order -> _leftQuantity  -= _leftQuantity;
        order -> _fillingQuantity = _leftQuantity;
        _fillingQuantity = _leftQuantity;
        _leftQuantity = 0;                
    }
    else
    {
        _leftQuantity  -= order -> _leftQuantity;
        order -> _fillingQuantity = order -> _leftQuantity;
        _fillingQuantity = order -> _leftQuantity;                
        order -> _leftQuantity = 0;
    }
    return true;
}

std::string Order::RenderAsFilled()
{
    std::ostringstream oss;
    oss << std::fixed;
    oss << std::setprecision(4);
    oss << "F " << GetOID() << " " << GetSymbol() << " " << GetFillingQuantity() << " " << GetPrice();
    return oss.str();
}


std::string Order::RenderAsCanceled()
{
    std::ostringstream oss;
    oss << "X " << GetOID();
    return oss.str();
}

std::string Order::RenderAsBlotter()
{
    std::ostringstream oss;
    oss << std::fixed;
    oss << std::setprecision(4);
    oss << "P " << GetOID() << " " << GetSymbol() << " " <<   GetSideStr() << " " << GetLeftQuantity() << " " << GetPrice();
    return oss.str();

}