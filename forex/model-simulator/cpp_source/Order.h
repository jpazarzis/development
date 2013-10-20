#ifndef ORDER_INCLUDED
#define ORDER_INCLUDED
#define ORDER_INCLUDED

/////////////////////////////////////////////////////
// Order.h
// 
// Author: John Pazarzis
//
// Date  : Sep. 28 2013
//
// Summary: 
//    The Order type is used by the model to create and monitor orders 
//
// Notes:
//
// As the code matures I will need to add a Trade class as well. For
// now I encapsulate the trading details within the Order
////////////////////////////////////////////////////////////////////////


#include "TickProcessor.h"
#include "std_include.h"

enum OrderType
{
    BUY, SELL
};

enum OrderStatus
{
    OPEN, CLOSED
};

class OrderPool;

class Order;

typedef Order& ORDER_REF ;
typedef Order* ORDER_PTR ;


enum ORDER_RESULT
{
    WIN, LOSS, REMAINS_OPEN
};

enum { DEFUALT_TIME_FRAME = 15};

class Order: public TickProcessor, virtual Identifiable
{
    protected:

        std::string _instrument;
        const double _stop_loss;
        const double _take_profit;
        double _buy_price;
        double _sell_price;
        OrderStatus _order_status;
        OrderType _order_type;
        const int _timeframe; // How many minutes to keep the order open

        static OrderPool _order_pool;

        bool reaching_stop_loss(const Tick& tick) const;
    
        bool reaching_take_profit(const Tick& tick) const;

        Order(  OrderType order_type, 
                const std::string& instrument, 
                double stop_loss, 
                double take_profit,
                double enter_price,
                const std::string& timestamp,
                int timeframe);

    public:

        virtual ~Order(); 

        static ORDER_PTR make( OrderType order_type, 
                            const std::string& instrument, 
                            double stop_loss, 
                            double take_profit, 
                            double enter_price,
                            const std::string& timestamp,
                            int timeframe = DEFUALT_TIME_FRAME );


        Order(const Order&);

        Order& operator=(const Order&);


        static void clear_order_pool();

        static int orders_count() ;

        virtual void process(const Tick& tick);

        std::string get_instrument() const;

        double get_stop_loss() const;

        double get_take_profit() const;

        double get_enter_price() const;

        OrderStatus get_order_status() const;

        OrderType get_order_type() const;

        double get_pnl() const;
    
        ORDER_RESULT get_win_or_loss() const;

        std::string to_string() const;

};

#endif
