///////////////////////////////////////////////////////////////////////////////////
// bftd.cpp - Build Forcast Training Data
// Author: John Pazarzis, Mon Nov 11 12:31:52 EST 2013
//
//
// This program prepares the training data to be used by a FANN to optimize its
// output. 
// /////////////////////////////////////////////////////////////////////////////////
//
// NOTES
//
// Price Normalization
//
//      Applies to a vector of prices. For example the bid prices of all the
//      ticks of the day V = {p0, p1, ... pn}
//
//      If MAX = MAX(V) and MIN = MIN(V)
//
//      Then the normalized representation of each data point in V will be as
//      follows:
//
//      NORM[i] (V[i] - MIN) / (MAX-MIN);
//
//      The open - high - low - close of the normal data will look as follows:
//
//      NORM[0] - 1 - 0 - NORM[n]
//
//      Based on this we only need the normal open - close to have a full
//      description of what is called candle data (since high and low are always
//      1 and 0)
//      
///////////////////////////////////////////////////////////////////////////////////
//
#include "CandleStickCollection.h"
#include "xmldocument.h"
#include <iostream>
using namespace std;

std::string make_training_data( const CandleStickCollection& csc, 
                       int history_index0,
                       int history_index1,
                       int future_index0,
                       int future_index1,
                       double delta){

    std::string strg;
    auto d = csc.get_window(history_index0, history_index1, trivial_normalizer);
    strg += to_string(d);
    strg += "\n";
    d = csc.get_window(future_index0, future_index1);
    strg += get_target_for_high_move(d,delta*0.0001);
    strg += "\n";
    return strg;
}

int main(int argc, char *argv[]){
    if(argc < 2)
    {
        cout << "correct use: " << argv[0] << "[configuration.xml]" << endl;
        return -1;
    }
    XmlDocument configuration(argv[1]);
    XmlNode& config = configuration["neural_network_training_data"];
    const string candles_file = config["candle_file"].value();
    const string output_file = config["output_file"].value();
    const int history_size = config["history_size"].value_to_int();
    const int future_size = config["future_size"].value_to_int();
    const double delta = config["delta"].value_to_double();
    CandleStickCollection csc(candles_file);
    int history_index0 = 0 , history_index1 = 0, future_index0 = 0, future_index1 = 0;
    vector<string> training_data;
    for(;;){
        history_index1 = history_index0 + history_size;
        future_index0 = history_index1;
        future_index1 =  future_index0 + future_size;
        if(future_index1 >= csc.size()){
            break;
        }
        training_data.push_back(make_training_data(csc, 
                                                  history_index0, 
                                                  history_index1, 
                                                  future_index0, 
                                                  future_index1,
                                                  delta));
        history_index0 = future_index1;
    }
    FILE *fp;
    fp=fopen(output_file.c_str(), "w");
    fprintf(fp,"%i %i 1\n", training_data.size(), history_size*4); 
    for(auto& s: training_data){
        fprintf(fp,s.c_str());
    }
    fclose(fp);
}

