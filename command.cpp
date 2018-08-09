#include "command.h"

bool CommandParser::IsDouble(const std::string& s)
{
    std::istringstream iss(s);
    double d;
    return iss >> d >> std::ws && iss.eof();
}

bool CommandParser::IsNumber(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

bool CommandParser::ParseOrder(std::vector<std::string> elems, Command& command)
{
    if (elems.size() != 6)
    {
        _lastErrorMsg = "Invalid command";
        return false;
    }
    if (!IsNumber(elems[1]))  // OID
    {
        _lastErrorMsg = "Invalid OID format";
        return false;
    }
    if (elems[2].length() > 8)  // symbol validation
    {
        _lastErrorMsg = "Invalid symbol notation";
        return false;
    }
    if (elems[3].length() != 1)  // order side
    {
        _lastErrorMsg = "Invalid Side";
        return false;
    }
    if (!IsNumber(elems[4]))  // quantity
    {
        _lastErrorMsg = "Invalid quantity format";
        return false;
    }
    if (!IsDouble(elems[5]))  // price
    {
        _lastErrorMsg = "Invalid price format";
        return false;
    }
    
    char orderSide = elems[3][0u];
    if (orderSide != 'S' && orderSide != 'B')
    {            
        _lastErrorMsg = "Invalid Side";
        return false;
    }

    command.Code = CommandCode::OrderCmd;
    command.detail.OID = std::stol(elems[1]);
    command.detail.Symbol = elems[2];
    command.detail.Quantity = std::stoi(elems[4]);
    command.detail.Price = std::stod(elems[5]);

    if (orderSide == 'S')
    {
        command.detail.Side = SideCode::Sell;
    }
    else
    {
        command.detail.Side = SideCode::Buy;
    }
    return true;
}

bool CommandParser::ParseCancellation(std::vector<std::string> elems, Command& command)
{
    if (elems.size() != 2)
    {
       _lastErrorMsg = "Invalid cancel command format";
        return false;
    }
    if (!IsNumber(elems[1]))  // OID
    {
        _lastErrorMsg = "Invalid OID format";
        return false;
    }
    command.Code = CommandCode::CancelCmd;
    command.detail.OID = std::stol(elems[1]);
    return true;
}

bool CommandParser::ParsePrint(std::vector<std::string> elems, Command& command)
{
    if (elems.size() != 1)
    {
        _lastErrorMsg = "Invalid Print command format";
        return false;
    }
    command.Code = CommandCode::PrintCmd;
    return true;
}

bool CommandParser::Parse(const std::string& line, Command& command)
{
    bool status = true;
    if (line.length() == 0)
    {
        _lastErrorMsg = "Invalid command format";
        return false;
    }

    std::vector<std::string> elems;
    std::istringstream iss(line);
    for(std::string s; iss >> s; )
        elems.push_back(s);

    switch(elems[0][0u])
    {                
        case OrderCodes::ORDER: 
            status = ParseOrder(elems, command);
            break;
        case OrderCodes::CANCEL:
            status = ParseCancellation(elems, command);
            break;
        case OrderCodes::PRINT:
            status = ParsePrint(elems, command);
            break;
        default:
            _lastErrorMsg = "Invalid command format";
            status = false;
    }               
    return status;
}

