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

#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"


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

using namespace boost::posix_time; 
using namespace boost::gregorian;

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
        const ptime _creation_time;
        const ptime _expiration_time;
        bool _was_expired;

        static OrderPool _order_pool;

        static int _expiration_minutes;

        bool reaching_stop_loss(const Tick& tick) const;
    
        bool reaching_take_profit(const Tick& tick) const;

        Order(  OrderType order_type, 
                const std::string& instrument, 
                double stop_loss, 
                double take_profit,
                double enter_price,
                const Tick& tick
                );

    public:

        virtual ~Order(); 

        static ORDER_PTR make( OrderType order_type, 
                            const std::string& instrument, 
                            double stop_loss, 
                            double take_profit, 
                            double enter_price,
                            const Tick& tick);


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

        bool was_expired() const;
    
        ORDER_RESULT get_win_or_loss() const;

        std::string to_string() const;

        static int get_expiration_minutes();

        static int set_expiration_minutes(int expiration_minutes);

};

#endif
