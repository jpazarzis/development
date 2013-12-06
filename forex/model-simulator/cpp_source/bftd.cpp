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

#include "toolkit.h"
#include <floatfann.h>
#include <fann.h>
#include "xmldocument.h"
#include <iostream>
#include "Logger.h"
#include <boost/algorithm/string.hpp>

using namespace std;


void train_neural_network(XmlNode& config){
    const string training_data_file = config["training_data_file"].value();
    const string neural_network_file = config["neural_network_file"].value();
    const float desired_error = (const float) config["desired_error"].value_to_double();
    const double hidden_neurons_factor = config["hidden_neurons_factor"].value_to_double();
    const unsigned int num_input = 96;
    const unsigned int num_output = 1;
    const unsigned int num_layers = config["number_of_layers"].value_to_int();;
    const unsigned int num_neurons_hidden = (int)num_input*hidden_neurons_factor;
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



         fann_destroy(ann);
     }
     myfile.close();

     cout << "success: " << successes << endl;
     cout << "failures: " << failures << endl;

     double total = successes + failures;

     printf("success : %10.2f\n", (successes /total)*100.0);

}

void update_trading_file(XmlNode& config) {
     const string filename = config["trading_file"].value();
     const string temp_filename = filename + ".to_execute";
     const string neural_network_file = config["neural_network_file"].value();
     FILE *fp =fopen(temp_filename.c_str(), "w");
     ifstream myfile (filename.c_str());
     string line;
     while ( getline (myfile,line) ) {
         fann_type *calc_out;
         fann_type input[96]; // change this to be dynamic instead of hardcoded
         struct fann *ann = fann_create_from_file(neural_network_file.c_str());
         vector<string> strs;
         boost::split(strs, line, boost::is_any_of(","));
         assert(strs.size() == 100);
         for(int i = 4; i < 100; ++i){
            input[i-4] = atof(strs[i].c_str());
         }
         calc_out = fann_run(ann, input);
         double c = calc_out[0];
         if(c >0.5){
             fprintf(fp,"%s,%s,%s,%s\n", strs[0].c_str(), strs[1].c_str(), strs[2].c_str(), strs[3].c_str());
         }
         fann_destroy(ann);
     }
     myfile.close();
     fclose(fp);
}

int main(int argc, char *argv[]){
    if(argc < 2)
    {
        cout << "correct use: " << argv[0] << "[configuration.xml]" << endl;
        return -1;
    }
    Logger::set_filename("forward_testing.log");
    XmlDocument configuration(argv[1]);
    XmlNode& config = configuration["neural_network_training_data"];
    train_neural_network(config);
    forward_test(config);
    update_trading_file(config);
}

