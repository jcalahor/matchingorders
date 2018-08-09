#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>

#pragma once

enum CommandCode {OrderCmd, CancelCmd, PrintCmd};   
enum ResultCode {Filled, Cancelled, BookDetail, Error};
enum SideCode {Buy, Sell};   


class OrderCodes
{
    public:
        const static char ORDER = 'O';
        const static char CANCEL = 'X';
        const static char PRINT = 'P';
};

struct OrderCommandDetail 
{
    long OID;
    std::string Symbol;
    SideCode Side;
    int Quantity;
    double Price;
};

struct Command 
{
    CommandCode Code;
    struct OrderCommandDetail detail;
};

struct Result
{
    ResultCode Code;
    struct OrderCommandDetail detail;
};


class CommandParser
{    
    bool IsDouble(const std::string& s);
    bool IsNumber(const std::string& s);
    bool ParseOrder(std::vector<std::string> elems, Command& command);
    bool ParseCancellation(std::vector<std::string> elems, Command& command);
    bool ParsePrint(std::vector<std::string> elems, Command& command);
    std::string _lastErrorMsg;

    public:
        bool Parse(const std::string& line, Command& command);   
        std::string& GetLastErrorMsg() { return _lastErrorMsg;}
};
