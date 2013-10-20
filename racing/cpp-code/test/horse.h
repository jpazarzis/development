// horse.h

#ifndef HORSE_INCLUDED
#define HORSE_INCLUDED

#include "pool.h"
#include "parsable.h"
#include "pastperformance.h"

#include <fstream>

#ifdef POOL_SIZE
#undef POOL_SIZE
#endif

enum HORSE_DATA_FIELD_INDEX
{
        RACE_TRACK = 0,
        RACE_DATE = 1,
        RACE_NUMBER = 2,
        POST_POSITION = 3,
        ENTRY_INDICATOR = 4,
        TODAYS_DISTANCE = 5,
        TODAYS_SURFACE = 6,
        RACE_TYPE = 8,
        AGE_SEX_RESTRICTIONS = 9,
        RACE_CLASSIFICATION = 10,
        RACE_CONDITIONS = 15,
        ALL_WEATHER_FLAG = 24,
        JOCKEY_NAME = 32,
        OWNERS_NAME = 38,
        OWNERS_SILKS = 39,
        MEDICATION = 61,
        EQUIPMENT_CHANGE = 63,
        PROGRAM_NUMBER = 42,
        ODDS = 43,
        HORSE_NAME = 44,
        YEAR_OF_BIRTH = 45,
        FOALING_MONTH = 46,
        HORSE_SEX = 48,
        HORSE_COLOR = 49,
        WEIGHT = 50,
        SIRE = 51,
        SIRES_SIRE = 52,
        DAM = 53,
        DAMS_SIRE = 54,
        STATE_WHERE_WAS_BRED = 56,
        UPDATED_POST_POSITION = 57,
        SIRES_STUD_FEE = 1176,
        LIFE_TIME_STARTS = 96,
        LIFE_TIME_WINS = 97,
        LIFE_TIME_PLACES = 98,
        LIFE_TIME_SHOWS = 99,
        LIFE_TIME_EARNINGS = 100,
        CURRENT_YEAR = 84,
        CURRENT_YEAR_STARTS = 85,
        CURRENT_YEAR_WINS = 86,
        CURRENT_YEAR_PLACES = 87,
        CURRENT_YEAR_SHOWS = 88,
        CURRENT_YEAR_EARNINGS = 89,
        PREVIOUS_YEAR = 90,
        PREVIOUS_YEAR_STARTS = 91,
        PREVIOUS_YEAR_WINS = 92,
        PREVIOUS_YEAR_PLACES = 93,
        PREVIOUS_YEAR_SHOWS = 94,
        PREVIOUS_YEAR_EARNINGS = 95,
        TODAYS_TRACK_STARTS = 69,
        TODAYS_TRACK_WINS = 70,
        TODAYS_TRACK_PLACES = 71,
        TODAYS_TRACK_SHOWS = 72,
        TODAYS_TRACK_EARNINGS = 73,
        WET_TRACK_STARTS = 79,
        WET_TRACK_WINS = 80,
        WET_TRACK_PLACES = 81,
        WET_TRACK_SHOWS = 82,
        WET_TRACK_EARNINGS = 83,
        TURF_TRACK_STARTS = 74,
        TURF_TRACK_WINS = 75,
        TURF_TRACK_PLACES = 76,
        TURF_TRACK_SHOWS = 77,
        TURF_TRACK_EARNINGS = 78,
        TODAYS_DISTANCE_STARTS = 64,
        TODAYS_DISTANCE_WINS = 65,
        TODAYS_DISTANCE_PLACES = 66,
        TODAYS_DISTANCE_SHOWS = 67,
        TODAYS_DISTANCE_EARNINGS = 68,
        BRIS_RUN_STYLE = 209,
        QUIRIN_SPEED_POINTS = 210,
        NUMBER_OF_DAYS_SINCE_LAST_RACE_FOR_TODAYS_RACE = 223,
        TODAYS_RACE_STATE_BRED = 238,
        CLAIMING_PRICE_OF_RACE = 12,
        CLAIMING_PRICE_OF_HORSE = 13,
        PRIME_POWER_RATING = 250,
        TRAINER = 27,
        TRAINER_STARTS = 28,
        TRAINER_WINS = 29,
        TRAINER_PLACES = 30,
        TRAINER_SHOWS = 31,
        BRIS_AVG_THREE_LAST_CLASS_RATINGS = 1145,
        BRIS_DIRT_PEDIGREE_RATING = 1263,
        BRIS_MUD_PEDIGREE_RATING = 1264,
        BRIS_TURF_PEDIGREE_RATING = 1265,
        BRIS_DISTANCE_PEDIGREE_RATING = 1266,
        POST_TIME = 1417
};

#define POOL_SIZE 5000
enum { MAX_NUMBER_OF_PAST_PERFORMANCES = 10 }; 

class horse: public poolable<horse,POOL_SIZE> 
{
    public:
    
        inline void populate(std::ifstream& stm)
        {
            _buffer.populate(stm);
        }

        inline size_t length() const
        {
            return _buffer.length();
        }

        inline std::string race_track() const
        {
            return _buffer.get_field_as_str(RACE_TRACK);
        }
        
        inline std::string race_date() const
        {
            return _buffer.get_field_as_str(RACE_DATE);
        }

        inline int race_number () const
        {
            return _buffer.get_field_as_num<int>(RACE_NUMBER );
        }

        inline int post_position () const
        {
            return _buffer.get_field_as_num<int>(POST_POSITION );
        }

        inline std::string entry_indicator () const
        {
            return _buffer.get_field_as_str(ENTRY_INDICATOR );
        }

        inline int todays_distance () const
        {
            return _buffer.get_field_as_num<int>(TODAYS_DISTANCE );
        }

        inline std::string todays_surface () const
        {
            return _buffer.get_field_as_str(TODAYS_SURFACE );
        }

        inline std::string race_type () const
        {
            return _buffer.get_field_as_str(RACE_TYPE );
        }

        inline std::string age_sex_restrictions () const
        {
            return _buffer.get_field_as_str(AGE_SEX_RESTRICTIONS );
        }

        inline std::string race_classification () const
        {
            return _buffer.get_field_as_str(RACE_CLASSIFICATION );
        }

        inline std::string race_conditions () const
        {
            return _buffer.get_field_as_str(RACE_CONDITIONS );
        }

        inline std::string all_weather_flag () const
        {
            return _buffer.get_field_as_str(ALL_WEATHER_FLAG );
        }

        inline std::string jockey_name () const
        {
            return _buffer.get_field_as_str(JOCKEY_NAME );
        }

        inline std::string owners_name () const
        {
            return _buffer.get_field_as_str(OWNERS_NAME );
        }

        inline std::string owners_silks () const
        {
            return _buffer.get_field_as_str(OWNERS_SILKS );
        }

        inline std::string medication () const
        {
            return _buffer.get_field_as_str(MEDICATION );
        }

        inline std::string equipment_change () const
        {
            return _buffer.get_field_as_str(EQUIPMENT_CHANGE );
        }

        inline std::string program_number () const
        {
            return _buffer.get_field_as_str(PROGRAM_NUMBER );
        }

        inline double odds () const
        {
            return _buffer.get_field_as_num<double>(ODDS );
        }

        inline std::string horse_name () const
        {
            return _buffer.get_field_as_str(HORSE_NAME );
        }

        inline int year_of_birth () const
        {
            return _buffer.get_field_as_num<int>(YEAR_OF_BIRTH );
        }

        inline int foaling_month () const
        {
            return _buffer.get_field_as_num<int>(FOALING_MONTH );
        }

        inline std::string horse_sex () const
        {
            return _buffer.get_field_as_str(HORSE_SEX );
        }

        inline std::string horse_color () const
        {
            return _buffer.get_field_as_str(HORSE_COLOR );
        }

        inline int weight () const
        {
            return _buffer.get_field_as_num<int>(WEIGHT );
        }

        inline std::string sire () const
        {
            return _buffer.get_field_as_str(SIRE );
        }

        inline std::string sires_sire () const
        {
            return _buffer.get_field_as_str(SIRES_SIRE );
        }

        inline std::string dam () const
        {
            return _buffer.get_field_as_str(DAM );
        }

        inline std::string dams_sire () const
        {
            return _buffer.get_field_as_str(DAMS_SIRE );
        }

        inline std::string state_where_was_bred () const
        {
            return _buffer.get_field_as_str(STATE_WHERE_WAS_BRED );
        }

        inline std::string updated_post_position () const
        {
            return _buffer.get_field_as_str(UPDATED_POST_POSITION );
        }

        inline int sires_stud_fee () const
        {
            return _buffer.get_field_as_num<int>(SIRES_STUD_FEE );
        }

        inline int life_time_starts () const
        {
            return _buffer.get_field_as_num<int>(LIFE_TIME_STARTS );
        }

        inline int life_time_wins () const
        {
            return _buffer.get_field_as_num<int>(LIFE_TIME_WINS );
        }

        inline int life_time_places () const
        {
            return _buffer.get_field_as_num<int>(LIFE_TIME_PLACES );
        }

        inline int life_time_shows () const
        {
            return _buffer.get_field_as_num<int>(LIFE_TIME_SHOWS );
        }

        inline int life_time_earnings () const
        {
            return _buffer.get_field_as_num<int>(LIFE_TIME_EARNINGS );
        }

        inline int current_year () const
        {
            return _buffer.get_field_as_num<int>(CURRENT_YEAR );
        }

        inline int current_year_starts () const
        {
            return _buffer.get_field_as_num<int>(CURRENT_YEAR_STARTS );
        }

        inline int current_year_wins () const
        {
            return _buffer.get_field_as_num<int>(CURRENT_YEAR_WINS );
        }

        inline int current_year_places () const
        {
            return _buffer.get_field_as_num<int>(CURRENT_YEAR_PLACES );
        }

        inline int current_year_shows () const
        {
            return _buffer.get_field_as_num<int>(CURRENT_YEAR_SHOWS );
        }

        inline int current_year_earnings () const
        {
            return _buffer.get_field_as_num<int>(CURRENT_YEAR_EARNINGS );
        }

        inline int previous_year () const
        {
            return _buffer.get_field_as_num<int>(PREVIOUS_YEAR );
        }

        inline int previous_year_starts () const
        {
            return _buffer.get_field_as_num<int>(PREVIOUS_YEAR_STARTS );
        }

        inline int previous_year_wins () const
        {
            return _buffer.get_field_as_num<int>(PREVIOUS_YEAR_WINS );
        }

        inline int previous_year_places () const
        {
            return _buffer.get_field_as_num<int>(PREVIOUS_YEAR_PLACES );
        }

        inline int previous_year_shows () const
        {
            return _buffer.get_field_as_num<int>(PREVIOUS_YEAR_SHOWS );
        }

        inline int previous_year_earnings () const
        {
            return _buffer.get_field_as_num<int>(PREVIOUS_YEAR_EARNINGS );
        }

        inline int todays_track_starts () const
        {
            return _buffer.get_field_as_num<int>(TODAYS_TRACK_STARTS );
        }

        inline int todays_track_wins () const
        {
            return _buffer.get_field_as_num<int>(TODAYS_TRACK_WINS );
        }

        inline int todays_track_places () const
        {
            return _buffer.get_field_as_num<int>(TODAYS_TRACK_PLACES );
        }

        inline int todays_track_shows () const
        {
            return _buffer.get_field_as_num<int>(TODAYS_TRACK_SHOWS );
        }

        inline int todays_track_earnings () const
        {
            return _buffer.get_field_as_num<int>(TODAYS_TRACK_EARNINGS );
        }

        inline int wet_track_starts () const
        {
            return _buffer.get_field_as_num<int>(WET_TRACK_STARTS );
        }

        inline int wet_track_wins () const
        {
            return _buffer.get_field_as_num<int>(WET_TRACK_WINS );
        }

        inline int wet_track_places () const
        {
            return _buffer.get_field_as_num<int>(WET_TRACK_PLACES );
        }

        inline int wet_track_shows () const
        {
            return _buffer.get_field_as_num<int>(WET_TRACK_SHOWS );
        }

        inline int wet_track_earnings () const
        {
            return _buffer.get_field_as_num<int>(WET_TRACK_EARNINGS );
        }

        inline int turf_track_starts () const
        {
            return _buffer.get_field_as_num<int>(TURF_TRACK_STARTS );
        }

        inline int turf_track_wins () const
        {
            return _buffer.get_field_as_num<int>(TURF_TRACK_WINS );
        }

        inline int turf_track_places () const
        {
            return _buffer.get_field_as_num<int>(TURF_TRACK_PLACES );
        }

        inline int turf_track_shows () const
        {
            return _buffer.get_field_as_num<int>(TURF_TRACK_SHOWS );
        }

        inline int turf_track_earnings () const
        {
            return _buffer.get_field_as_num<int>(TURF_TRACK_EARNINGS );
        }

        inline int todays_distance_starts () const
        {
            return _buffer.get_field_as_num<int>(TODAYS_DISTANCE_STARTS );
        }

        inline int todays_distance_wins () const
        {
            return _buffer.get_field_as_num<int>(TODAYS_DISTANCE_WINS );
        }

        inline int todays_distance_places () const
        {
            return _buffer.get_field_as_num<int>(TODAYS_DISTANCE_PLACES );
        }

        inline int todays_distance_shows () const
        {
            return _buffer.get_field_as_num<int>(TODAYS_DISTANCE_SHOWS );
        }

        inline int todays_distance_earnings () const
        {
            return _buffer.get_field_as_num<int>(TODAYS_DISTANCE_EARNINGS );
        }

        inline std::string bris_run_style () const
        {
            return _buffer.get_field_as_str(BRIS_RUN_STYLE );
        }

        inline int quirin_speed_points () const
        {
            return _buffer.get_field_as_num<int>(QUIRIN_SPEED_POINTS );
        }

        inline int number_of_days_since_last_race_for_todays_race () const
        {
            return _buffer.get_field_as_num<int>(NUMBER_OF_DAYS_SINCE_LAST_RACE_FOR_TODAYS_RACE );
        }

        inline std::string todays_race_state_bred () const
        {
            return _buffer.get_field_as_str(TODAYS_RACE_STATE_BRED );
        }

        inline int claiming_price_of_race () const
        {
            return _buffer.get_field_as_num<int>(CLAIMING_PRICE_OF_RACE );
        }

        inline int claiming_price_of_horse () const
        {
            return _buffer.get_field_as_num<int>(CLAIMING_PRICE_OF_HORSE );
        }

        inline int prime_power_rating () const
        {
            return _buffer.get_field_as_num<int>(PRIME_POWER_RATING );
        }

        inline std::string trainer () const
        {
            return _buffer.get_field_as_str(TRAINER );
        }

        inline int trainer_starts () const
        {
            return _buffer.get_field_as_num<int>(TRAINER_STARTS );
        }

        inline int trainer_wins () const
        {
            return _buffer.get_field_as_num<int>(TRAINER_WINS );
        }

        inline int trainer_places () const
        {
            return _buffer.get_field_as_num<int>(TRAINER_PLACES );
        }

        inline int trainer_shows () const
        {
            return _buffer.get_field_as_num<int>(TRAINER_SHOWS );
        }
        
        inline int bris_avg_three_last_class_ratings () const
        {
            return _buffer.get_field_as_num<int>(BRIS_AVG_THREE_LAST_CLASS_RATINGS );
        }

        inline int bris_dirt_pedigree_rating () const
        {
            return _buffer.get_field_as_num<int>(BRIS_DIRT_PEDIGREE_RATING );
        }

        inline int bris_mud_pedigree_rating () const
        {
            return _buffer.get_field_as_num<int>(BRIS_MUD_PEDIGREE_RATING );
        }

        inline int bris_turf_pedigree_rating () const
        {
            return _buffer.get_field_as_num<int>(BRIS_TURF_PEDIGREE_RATING );
        }

        inline int bris_distance_pedigree_rating () const
        {
            return _buffer.get_field_as_num<int>(BRIS_DISTANCE_PEDIGREE_RATING );
        }

        inline std::string post_time () const
        {
            return _buffer.get_field_as_str(POST_TIME );
        }

        // Past performances

        inline size_t count_past_performances() const
        {
            size_t counter = 0; 
            for(register int i=0; i <MAX_NUMBER_OF_PAST_PERFORMANCES; ++i)
                counter += _pastperformances[i].isvalid() ? 1 : 0;
            return counter;
        }

        inline const pastperformance& get_past_performance(size_t index) const
        {
            return _pastperformances[index];    
        }

    private:

        horse() 
        {
            for(register int i=0; i < MAX_NUMBER_OF_PAST_PERFORMANCES; ++i)
            {
                _pastperformances.push_back(pastperformance(&_buffer,i)); 
            }
        }

        std::vector<pastperformance> _pastperformances;

        parsable _buffer;

        horse(const horse& other);      
        
        horse& operator=(const horse& other);

        friend class poolable;
};

#endif // HORSE_INCLUDED 
