// FitnessStatistics.h
//
// Author        : John Pazarzis
// Creation date : Mon 28 Oct 2013 10:22:32 AM EDT
//
// Summary
//      Based in a provided set of orders calculates the fitness of the model
//      that generated them including several other indicative statistics

#ifndef FITNESSSTATISTICS_INCLUDED
#define FITNESSSTATISTICS_INCLUDED

#include "Order.h"
#include "Statistics.h"
#include <vector>
#include "std_include.h"
#include <assert.h>

class FitnessStatistics final
{
                double _max_draw_down;
                double _account_balance;
                double _absolute_low;
                double _absolute_high;
                double _fitness;
                double _pnl;
                int _number_of_orders;
                int _expired_trades;
                int _winning_expired_trades;
                int _losing_expired_trades;
                int _winning_trades;
                int _lossing_trades;

                

        public:

                    FitnessStatistics(   double maxdrawdown = 0.0,
                                 double accountbalance = 0.0,
                                 double absolutelow = 0.0,
                                 double absolutehigh = 0.0,
                                 double fitness = 0.0,
                                 double pnl = 0.0,
                                 int numberoforders = 0,
                                 int expiredtrades = 0,
                                 int winningexpiredtrades = 0,
                                 int losingexpiredtrades = 0,
                                 int winningtrades = 0,
                                 int lossingtrades = 0):

                 _max_draw_down(maxdrawdown),
                 _account_balance(accountbalance),
                 _absolute_low(absolutelow),
                 _absolute_high(absolutehigh),
                 _fitness(fitness),
                 _pnl(pnl),
                 _number_of_orders(numberoforders),
                 _expired_trades(expiredtrades),
                 _winning_expired_trades(winningexpiredtrades),
                 _losing_expired_trades(losingexpiredtrades),
                 _winning_trades(winningtrades),
                 _lossing_trades(lossingtrades)

         {
         }



                static FitnessStatistics make(std::vector<Order*> orders)
                {
                    using namespace std;

                    double maxdrawdown = 0.0;
                    double accountbalance = 50000;
                    double absolutelow = 0.0;
                    double absolutehigh = 0.0;
                    double fitness = 0.0;
                    const int numberoforders = orders.size();
                    int expiredtrades = 0;
                    int winningexpiredtrades = 0;
                    int losingexpiredtrades = 0;
                    int winningtrades = 0;
                    int lossingtrades = 0;

                    if(numberoforders <= 30)
                    {
                        return FitnessStatistics();
                    }
                    else
                    {
                            std::vector<double> account_balance_curve;
                            account_balance_curve.push_back(accountbalance);

                            for(int i = 0; i < numberoforders; ++i)
                            {
                                const double trade_pnl = orders[i]->get_pnl();

                                if(orders[i]->was_expired())
                                {
                                    ++expiredtrades;
                                    if(trade_pnl > 0)
                                        ++winningexpiredtrades;
                                    else if(trade_pnl < 0)
                                     ++losingexpiredtrades;
                                }
                                else if(trade_pnl > 0)
                                    ++winningtrades;
                                else if(trade_pnl < 0)
                                     ++lossingtrades;

                                accountbalance += trade_pnl;

                                if(accountbalance <= 0)
                                {   
                                    return FitnessStatistics();     
                                }

                                account_balance_curve.push_back(accountbalance);
                            }

                            maxdrawdown = max_drawdown(account_balance_curve);
                            absolutelow = *std::min_element(account_balance_curve.begin(), account_balance_curve.end()); 
                            assert(accountbalance > 0);
                            assert(maxdrawdown >= 0);

                            const double pnl = accountbalance - 50000;

                            double effective_balance = accountbalance + 3.0 * pnl;

                            // In case that the max drawdown is zero just ignore the chromosome
                            // this is a case that will happen when there is not a single loosing
                            // trade something that needs a very small number of trades
                            // Note that a very low _max_draw_down will result to a high
                            // fitness creating a bias for models creating very few
                            // orders (since they are more possible to create a very
                            // small amount of lossing trades..
                            if(maxdrawdown <= 0.001 || effective_balance <= 0)
                            {
                                    return FitnessStatistics();     
                            }
                            else
                            {
                                fitness = effective_balance*1.0;     

                                return FitnessStatistics( maxdrawdown,
                                                          accountbalance,
                                                          absolutelow,
                                                          absolutehigh,
                                                          fitness,
                                                          pnl,
                                                          numberoforders,
                                                          expiredtrades,
                                                          winningexpiredtrades,
                                                          losingexpiredtrades,
                                                          winningtrades,
                                                          lossingtrades );
                            }
                    }
                }

                inline double max_draw_down() const { return _max_draw_down;}

                inline double account_balance() const { return _account_balance;}

                inline double absolute_low() const { return _absolute_low;}

                inline double absolute_high() const { return _absolute_high;}

                inline double fitness() const { return _fitness;}

                inline double pnl() const { return _pnl;}

                inline int number_of_orders() const { return _number_of_orders;}

                inline int expired_trades() const { return _expired_trades;}

                inline int winning_expired_trades() const { return _winning_expired_trades;}

                inline int losing_expired_trades() const { return _losing_expired_trades;}

                inline int winning_trades() const { return _winning_trades;}

                inline int lossing_trades() const { return _lossing_trades;}


                std::string get_full_description() const
                {
                    std::string strg;

                    strg += sformat("number of orders:", "%20s");
                    strg += sformat(_number_of_orders,"%20d"); 
                    strg += "\n";

                    strg += sformat("final balance:", "%20s");
                    strg += sformat(_account_balance, "%20.2f");
                    strg += "\n";

                    strg += sformat("final pnl:", "%20s");
                    strg += sformat(_pnl, "%20.2f");
                    strg += "\n";

                    strg += sformat("low:", "%20s");
                    strg += sformat(_absolute_low, "%20.2f");
                    strg += "\n";

                    strg += sformat("winning trades:", "%20s");
                    strg += sformat(_winning_trades, "%20d");
                    strg += "\n";

                    strg += sformat("losing trades:", "%20s");
                    strg += sformat(_lossing_trades, "%20d");
                    strg += "\n";

                    strg += sformat("expired trades total:", "%20s");
                    strg += sformat(_expired_trades, "%20d");
                    strg += "\n";

                    strg += sformat("winning expired count:", "%20s");
                    strg += sformat(_winning_expired_trades, "%20d");
                    strg += "\n";

                    strg += sformat("losing expired count:", "%20s");
                    strg += sformat(_losing_expired_trades, "%20d");
                    strg += "\n";

                    strg += sformat("max drawdown:", "%20s");
                    strg += sformat(_max_draw_down, "%20.5f");
                    strg += "\n";

                    return strg;
                }
};

#endif // FITNESSSTATISTICS_INCLUDED
