#include <string>
#include <fstream>
#include <iostream>
#include <list>
#include <vector>
#include <sstream>
#include <algorithm>
#include <map>
#include <memory>
#include "order.h"
#include "command.h"
#include "types.h"
#include "order_blotter.h"
#include "order_filler.h"
#include "orders_container.h"


class SimpleCross
{    
    OrdersContainer* _container;
    OrderFiller* _filler;
    OrderBlotter* _blotter;

    void AttachToResults(results_t& results, std::string msg);
    void AttachErrorToResults(results_t& results, std::string msg);
    void ProcessOrderCommand(results_t& results, Command& cmd);
    void ProcessCancelCommand(results_t& results, Command& cmd);
    void ProcessBlotterCommand(results_t& results, Command& cmd);

    public:
        SimpleCross()
        {
            _container = new OrdersContainer();
            _filler = new OrderFiller(_container);
            _blotter = new OrderBlotter(_container);
        }

        ~SimpleCross()
        {
            _container -> Clear();
            delete _container;
            delete _filler;
            delete _blotter;
        }

        results_t action(const std::string& line) 
        { 
            CommandParser parser;
            Command cmd;
            results_t results;
            if (parser.Parse(line, cmd))
            {
                switch(cmd.Code)
                {
                    case CommandCode::OrderCmd:                    
                        ProcessOrderCommand(results, cmd);
                        break;                        
                    case CommandCode::CancelCmd:
                        ProcessCancelCommand(results, cmd);
                        break;
                    case CommandCode::PrintCmd:
                        ProcessBlotterCommand(results, cmd);
                        break;
                }
            }
            else
            {
                 AttachErrorToResults(results, parser.GetLastErrorMsg());
            }
            results.push_front("results.size() == " + std::to_string(results.size()));
            return results;          
        }
};


void SimpleCross::AttachToResults(results_t& results, std::string msg)
{
    results.push_back("results[" + std::to_string(results.size()) + "] ==  \"" + msg + "\"");
}

void SimpleCross::AttachErrorToResults(results_t& results, std::string msg)
{
    results.push_back("results[" + std::to_string(results.size()) + "] ==  E \"" + msg + "\"");
}


void SimpleCross::ProcessOrderCommand(results_t& results, Command& cmd)
{
    Order* order;
    order = new Order(cmd.detail.OID, cmd.detail.Symbol, cmd.detail.Side, cmd.detail.Quantity, cmd.detail.Price);
    if (_container -> StoreOrder(order))
    {
        _filler -> TryFill(order);      
        for (auto item : _filler -> GetFillingOrders())
        {
            AttachToResults(results, item -> RenderAsFilled());
        } 
    }
    else
    {
        AttachErrorToResults(results, _container -> GetLastErrorMsg());
    }   
}

void SimpleCross::ProcessCancelCommand(results_t& results, Command& cmd)
{
    Order* order;
    order = new Order(cmd.detail.OID, cmd.detail.Symbol, cmd.detail.Side, cmd.detail.Quantity, cmd.detail.Price);
    if (_container -> Cancel(order))
    {
        AttachToResults(results, _container -> GetLastCanceleOrder() -> RenderAsCanceled());
    }
    else
    {
        AttachErrorToResults(results, _container -> GetLastErrorMsg());
    }
}

void SimpleCross::ProcessBlotterCommand(results_t& results, Command& cmd)
{
    _blotter -> BuildBlotter();
    for (auto item : _blotter -> GetCurrentBlotter())
    {
        AttachToResults(results, item -> RenderAsBlotter());
    } 
}


void ReadFromActionsFileTest()
{
    SimpleCross scross;
    std::string line;
    std::ifstream actions("actions.txt", std::ios::in);

    while (std::getline(actions, line))
    {
        std::cout << "processing line ->" << line << "\n";
        getchar();
        results_t results = scross.action(line);
        for (results_t::const_iterator it=results.begin(); it!=results.end(); ++it)
        {
            std::cout << *it << std::endl;
        }
    }
}

void Assert(bool condition, const std::string& msg)
{
    if (!condition)
    {
        std::cout << "\n" << msg;
    }
}


void PerformUnitTest()
{
    ///////////////
    // order class tests
    //////////////
    Order* order = new Order(1000, "IBM", SideCode::Sell, 100, 200);
    Assert(order -> GetOID() == 1000, "Invalid OID");
    std::string renderMsg = order -> RenderAsBlotter();
    Assert(renderMsg == "P 1000 IBM S 100 200.0000", "RenderAsBlotter failed");
    Assert(order -> GetLeftQuantity() == 100, "Invalid Left Quantity");

    // test simple fill
    Order* order2 = new Order(1001, "IBM", SideCode::Buy, 50, 200);
    bool status = order -> TryFill(order2);
    Assert(order -> GetFillingQuantity() == 50, "Invalid filling quantity");

    Order* order3 = new Order(1002, "IBM", SideCode::Sell, 50, 200);
    status = order -> TryFill(order3);
    Assert(status == false, "Invalid filling sell to sell order");


    // test cancellation
    Assert(order -> IsCanceled() == false, "Invalid cancel status before cancellation");
    order -> CancelOrder();
    Assert(order -> IsCanceled() == true, "Invalid cancel status after cancellation");
    delete order;
    delete order2;
    delete order3;


    ///////////////
    // order container class tests
    //////////////
    OrdersContainer* container = new OrdersContainer();
    order = new Order(1000, "IBM", SideCode::Sell, 100, 200);
    container -> StoreOrder(order);

    auto bucketBySymbol =  container -> GetBucketsBySymbolObj();
    auto pos = bucketBySymbol.find(order -> GetSymbol());
    Assert( pos != bucketBySymbol.end(), "Order Symbol should exists in container");
    auto priceBucket = pos -> second;
    auto posb = priceBucket -> find(order -> GetPrice());
    std::list<Order*>* ords = posb -> second;
    Order* o = ords -> front();
    Assert( o -> GetOID() == order -> GetOID(), "Order OID should exists in container");
    
    delete container;
    delete order;


    ///////////////
    // Order filler class tests
    //////////////
    container = new OrdersContainer();
    OrderFiller* filler = new OrderFiller(container);
    order = new Order(1000, "IBM", SideCode::Sell, 100, 200);
    order2 = new Order(1001, "IBM", SideCode::Buy, 50, 200);
    container -> StoreOrder(order);
    container -> StoreOrder(order2);
    filler -> TryFill(order2);
    Assert(filler -> GetFillingOrders().size() == 2, "Invalid number of filling orders;");
    Assert(order -> GetFillingQuantity() == 50, "Invalid filling quantity");
    Assert(order2 -> GetLeftQuantity() == 0, "Invalid left quantity");
    order3 = new Order(1010, "MSFT", SideCode::Buy, 100, 180);
    Order* order4 = new Order(1011, "MSFT", SideCode::Sell, 100, 200);
    container -> StoreOrder(order3);
    container -> StoreOrder(order4);
    filler -> TryFill(order3);
    Assert(filler -> GetFillingOrders().size() == 0, "Invalid number of filling orders shoudl be zero");
    Assert(order3 -> GetLeftQuantity() == 100, "Invalid left quantity should be 100");

    delete container;
    delete filler;
    delete order;
    delete order2;
    delete order3;
    delete order4;

    ///////////////////////////
    // Blotter class unit tests
    /////////////////////////
    container = new OrdersContainer();
    order = new Order(1000, "IBM", SideCode::Sell, 100, 200);
    order2 = new Order(1002, "IBM", SideCode::Sell, 100, 200);
    OrderBlotter* blotter = new OrderBlotter(container);
    container -> StoreOrder(order);
    container -> StoreOrder(order2);
    blotter -> BuildBlotter();
    auto currentOrders = blotter -> GetCurrentBlotter();
    Assert(currentOrders.size() == 2, "Invalid number of orders on blotter");

    delete container;
    delete order;
    delete order2;
    delete blotter;


    ////////////////
    std::cout << "\nTests Done\n";
}

int main(int argc, char **argv)
{
    if (argc == 1 )
    {        
        ReadFromActionsFileTest();
    }
    else
    {
        PerformUnitTest();
    }
    return 0;
}