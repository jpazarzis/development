// FreeMarginModel.h
//
// Author        : John Pazarzis
// Creation date : Fri 08 Nov 2013 08:32:37 AM EST
//
// Summary
//      Implements a sample Free Margin Model
//

#ifndef FREEMARGINMODEL_INCLUDED
#define FREEMARGINMODEL_INCLUDED

#include "Position.h"
#include "TickPool.h"
#include <memory>
#include <vector>
#include <assert.h>

class FreeMarginModel {
        std::vector<std::unique_ptr<Position>> _open_positions;
        std::vector<std::unique_ptr<Position>> _closed_positions;
        double _balance;
        const int _leverage;
        const int _lot_size;
        POSITION_FACTORY _position_factory;
        double _reentering_pips;
        double _reentering_level;
        Tick* _active_tick;
        
    public:
        FreeMarginModel(double starting_balance, 
                        int leverage, 
                        double reentering_pips,
                        POSITION_FACTORY position_factory) :
            _balance(starting_balance),
            _leverage(leverage),
            _reentering_pips(reentering_pips),
            _position_factory(position_factory),
            _lot_size(100000),
            _reentering_level(0),
            _active_tick(NULL) {
        }

        virtual ~FreeMarginModel() {
        }

        FreeMarginModel(const FreeMarginModel& other) = delete;

        FreeMarginModel& operator=(const FreeMarginModel& other) = delete;

        void start_processising() {
            printf("%12s %12s %12s %12s %12s\n","bid","ask","balance","equity","freemargin");
            TickPool& tick_pool = TickPool::singleton();
            assert(tick_pool.size() > 1);
            _active_tick = &tick_pool[0];
            assert(NULL != _active_tick);
            _open_positions.clear();
            open_position();
            for(int i = 1; i < tick_pool.size(); ++i)
            {
                _active_tick = &tick_pool[i];
                assert(NULL != _active_tick);
                process_tick();
            }
        }

        double balance() const {
            return _balance;
        }

        double margin() const{
            double m = 0;
            for(auto& p: _open_positions)
                m += p->entering_cost();
            return m;
        }

        double equity() const{
            double e = _balance;
            if(NULL != _active_tick)
            {
                for(auto& p: _open_positions)
                    e +=  p->floating_profit(*_active_tick);
            }
            return e;
        }

        double free_margin() const {
            return equity() - margin();
        }


        std::string to_string() const {
            std::string strg("FreeMarginModel\n");
            strg += Position::header() + "\n";
            for(auto& p: _closed_positions) {
                strg += p->to_string() + "\n";
            }
            return strg;
        }


    private:

        void open_position(){
            assert(NULL != _active_tick);
            const double number_of_lots = (free_margin() * 0.8) * _leverage / (_lot_size * _active_tick->ask());
            _reentering_level = _active_tick->bid() + (_reentering_pips / 10000.0);
            _open_positions.push_back(_position_factory(*_active_tick, number_of_lots, _lot_size));
            print_info();

        }

        void print_info()
        {
            printf("%20s %12.5f %12.5f %12.0f %12.0f %12.0f\n",format_datetime(_active_tick->timestamp()).c_str()  ,_active_tick->bid(),_active_tick->ask(),_balance,equity(),free_margin());
        }

        void close_all_open_positions(){
            printf("closing all positions\n");
            for(auto& p: _open_positions)
            {
                p->close(*_active_tick);
                _balance += p->pnl();
            }

            for(auto& p: _open_positions)
                _closed_positions.push_back(move(p));

            _open_positions.clear();
        }



       void process_tick(){ 
           assert(NULL != _active_tick);
            
           if(free_margin() <= 0.0){
                close_all_open_positions();
           }


           if(_active_tick->bid() >= _reentering_level){
                open_position();
           }
           print_info();
        }
};
#endif // FREEMARGINMODEL_INCLUDED
