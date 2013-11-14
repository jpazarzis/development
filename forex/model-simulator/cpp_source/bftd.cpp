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
#include "toolkit.h"
#include <floatfann.h>
#include <fann.h>
#include "xmldocument.h"
#include <iostream>


using namespace std;

std::string make_training_data_line( const CandleStickCollection& csc, 
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
    return strg;
}

void prepare_data_files(XmlNode& config){
    const string candles_file = config["candle_file"].value();
    const string training_data_file = config["training_data_file"].value();
    const string forward_test_file = config["forward_test_file"].value();
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
        training_data.push_back(make_training_data_line(csc, 
                                                  history_index0, 
                                                  history_index1, 
                                                  future_index0, 
                                                  future_index1,
                                                  delta));
        history_index0 = future_index1;
    }

    const int split_index = training_data.size() * 0.80;

    FILE *fp;
    fp=fopen(training_data_file.c_str(), "w");
    fprintf(fp,"%i %i 1\n", split_index, history_size*4); 
    for(int i=0; i < split_index; ++i){
        fprintf(fp,"%s\n",training_data[i].c_str());
    }
    fclose(fp);

    fp=fopen(forward_test_file.c_str(), "w");
    for(int i=split_index; i < training_data.size(); ++i){
        fprintf(fp,"%s\n",training_data[i].c_str());
    }
    fclose(fp);
}

void train_neural_network(XmlNode& config){
    const int history_size = config["history_size"].value_to_int();
    const string training_data_file = config["training_data_file"].value();
    const string neural_network_file = config["neural_network_file"].value();
    const float desired_error = (const float) config["desired_error"].value_to_double();
    const double hidden_neurons_factor = config["hidden_neurons_factor"].value_to_double();
    const unsigned int num_input = history_size*4;
    const unsigned int num_output = 1;
    const unsigned int num_layers = config["number_of_layers"].value_to_int();;
    const unsigned int num_neurons_hidden = (int)history_size*hidden_neurons_factor;
    const unsigned int max_epochs = config["max_epochs"].value_to_int();
    const unsigned int epochs_between_reports = config["epochs_between_reports"].value_to_int();

    struct fann *ann = fann_create_standard(num_layers, num_input,
        num_neurons_hidden, num_output);

    fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
    fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);
    fann_train_on_file(ann, training_data_file.c_str(), max_epochs,
        epochs_between_reports, desired_error);

    fann_save(ann, neural_network_file.c_str());
    fann_destroy(ann);
}

void forward_test(XmlNode& config){
     const string forward_test_file = config["forward_test_file"].value();
     const string neural_network_file = config["neural_network_file"].value();
     const int history_size = config["history_size"].value_to_int();

     ifstream myfile (forward_test_file.c_str());
     string line;
     int successes = 0, failures = 0;
     while ( getline (myfile,line) )
     {
        fann_type *calc_out;
        fann_type input[96]; // change this to be dynamic instead of hardcoded

        trim(line);

        

        struct fann *ann = fann_create_from_file(neural_network_file.c_str());
         vector<string> strs;
         boost::split(strs, line, boost::is_any_of(" "));
         assert(strs.size() == 96);
         for(int i = 0; i < 96; ++i){
            input[i] = atof(strs[i].c_str());
         }
         calc_out = fann_run(ann, input);
         getline (myfile,line);
         boost::split(strs, line, boost::is_any_of("\t "));
         assert(strs.size() == 1);
         
         printf("%8.2f ", calc_out[0]);
         printf("%s \n", strs[0].c_str());

         double c = calc_out[0];
         int desired = atoi(strs[0].c_str());

         if(c > 0.5 && desired == 1){
             ++successes;
         }

         if(c > 0.5 && desired == 0){
             ++failures;
         }

         if(c < 0.5 && desired == 1){
             ++failures;
         }

         if(c < 0.5 && desired == 0){
             ++successes;
         }


         fann_destroy(ann);
     }
     myfile.close();

     cout << "success: " << successes << endl;
     cout << "failures: " << failures << endl;
}

int main(int argc, char *argv[]){
    if(argc < 2)
    {
        cout << "correct use: " << argv[0] << "[configuration.xml]" << endl;
        return -1;
    }
    XmlDocument configuration(argv[1]);
    XmlNode& config = configuration["neural_network_training_data"];

    //prepare_data_files(config);
    //train_neural_network(config);
    forward_test(config);
}

