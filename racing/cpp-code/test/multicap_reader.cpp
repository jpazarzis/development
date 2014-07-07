#include <sys/types.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include "utilities.h"
#include "Starter.h"
#include <iostream>
#include <iterator>
#include <algorithm>
#include "statutils.h"
#include "horse.h"
#include "handicappingfactors.h"
#include <stdlib.h>
#include <stdio.h>
using namespace std;
const char* pp_dir = "/home/john/samples3/2011";

std::vector<HandicappingFactor*> factors;
int total_number_of_starters = 0;
int number_of_cards = 0;
int number_of_races = 0;


void calculate_handicapping_factors(const std::vector<horse*>& horses) {
    register int current_race = -1;

    for(register int i = 0; i < horses.size(); ++i) {
        
        if (horses[i]->race_number() != current_race){
            ++number_of_races;
            current_race = horses[i]->race_number();
        }

        ++total_number_of_starters;
        for(register int j = 0; j < factors.size(); ++j) {
            factors[j]->update(*horses[i]);
        }
    }
}

void process_pp4(const std::string& filename)
{
    if(filename[0] == '.') {
        return;
    }

    int max = 0;
    ifstream myfile(filename.c_str());
    std::vector<horse*> horses;
    {
        while(myfile.good()) {
            horse* h = horse::get();
            h->populate(myfile);

            if(h->length() <= 0) {
                horse::put_back(h);
                continue;
            }

            horses.push_back(h);
            const int l = h->length();

            if(l > max) {
                max = l;
            }
        }

        myfile.close();
    }

    calculate_handicapping_factors(horses);


    for(register int i = 0; i < horses.size(); ++i) {
        horse::put_back(horses[i]);
    }
}


void process_directory(const std::string& directory){
    string dir = string(directory);
    vector<string> files = vector<string>();
    getdir(dir, files);
    for(unsigned int i = 0; i < files.size(); i++) {
        if(files[i][0] == '.') {
            continue;
        }
        process_pp4(string(pp_dir) + string("/") + files[i]);
        ++number_of_cards;
    }
}

int main()
{
    cout <<"started at: "<< timestamp() << endl;

    time_t start_time;
    time(&start_time);  

    factors.push_back(new ComingOfALayoff());
    factors.push_back(new BigWinnerLastOut());
    factors.push_back(new WinnerLastOut());
    factors.push_back(new RunSecondLastOut());

    process_directory("/home/john/samples3/2008");
    process_directory("/home/john/samples3/2009");
    process_directory("/home/john/samples3/2010");
    process_directory("/home/john/samples3/2011");
    process_directory("/home/john/samples3/2012");

    std::cout << endl ;
    std::cout << "TOTAL number of cards : " << number_of_cards << std::endl;
    std::cout << "TOTAL number of races : " << number_of_races << std::endl;

    std::cout << "TOTAL number of horses : " << total_number_of_starters << std::endl;

    std::cout << endl ;
    std::cout << "HANDICAPPING FACTORS" << endl;
    std::cout << "====================" << endl;
    std::cout << endl ;
    

    for(register int j = 0; j < factors.size(); ++j) {
        std::cout << factors[j]->tostring() << std::endl;
    }

    std::cout << endl ;

    

    std::cout << endl ;

    cout <<"finished at: "<< timestamp() << endl;

    time_t end_time;
    time(&end_time);  

    const double seconds = difftime(end_time, start_time );
    std::cout << "Duration is seconds: " <<  seconds << endl;
    std::cout << "Time Per race: " <<  (double) seconds / (double) number_of_races << endl;
    std::cout << "Races Per Second: " <<  (double) number_of_races / (double) seconds << endl;

    return 0;
}
