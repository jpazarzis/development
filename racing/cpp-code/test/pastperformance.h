// pastperformance.h
#ifndef PASTPERFORMANCE_INCLUDED
#define PASTPERFORMANCE_INCLUDED

class pastperformance
{
    public:

        pastperformance(parsable* pbuffer, int index):
            _pbuffer(pbuffer), _index(index)
        {
        }

        pastperformance(const pastperformance& other)
            : _pbuffer(other._pbuffer), _index(other._index)
        {
        }

        pastperformance& operator=(const pastperformance& other)
        {
            if(this != &other)
            {
                _pbuffer = other._pbuffer;
                _index = other._index;
            }

            return *this;
        }

        inline bool isvalid() const
        {
            return 8 == date().length();
        }   

            inline std::string date() const
            {
                return _pbuffer->get_field_as_str(DATE + _index);
            }

            inline int days_since_last() const
            {
                return _pbuffer->get_field_as_num<int>(DAYS_SINCE_LAST + _index);
            }

            inline std::string race_track() const
            {
                return _pbuffer->get_field_as_str(RACE_TRACK + _index);
            }

            inline int race_number() const
            {
                return _pbuffer->get_field_as_num<int>(RACE_NUMBER + _index);
            }

            inline std::string track_condition() const
            {
                return _pbuffer->get_field_as_str(TRACK_CONDITION + _index);
            }

            inline int distance() const
            {
                return _pbuffer->get_field_as_num<int>(DISTANCE + _index);
            }

            inline std::string surface() const
            {
                return _pbuffer->get_field_as_str(SURFACE + _index);
            }

            inline int number_of_entrants() const
            {
                return _pbuffer->get_field_as_num<int>(NUMBER_OF_ENTRANTS + _index);
            }

            inline int post_position() const
            {
                return _pbuffer->get_field_as_num<int>(POST_POSITION + _index);
            }

            inline std::string equipment() const
            {
                return _pbuffer->get_field_as_str(EQUIPMENT + _index);
            }

            inline std::string medication() const
            {
                return _pbuffer->get_field_as_str(MEDICATION + _index);
            }

            inline std::string trip_comment() const
            {
                return _pbuffer->get_field_as_str(TRIP_COMMENT + _index);
            }

            inline std::string winners_name() const
            {
                return _pbuffer->get_field_as_str(WINNERS_NAME + _index);
            }

            inline std::string second_place_finisher_name() const
            {
                return _pbuffer->get_field_as_str(SECOND_PLACE_FINISHER_NAME + _index);
            }

            inline std::string third_place_finisher_name() const
            {
                return _pbuffer->get_field_as_str(THIRD_PLACE_FINISHER_NAME + _index);
            }

            inline int winners_weight() const
            {
                return _pbuffer->get_field_as_num<int>(WINNERS_WEIGHT + _index);
            }

            inline int second_place_finisher_weight() const
            {
                return _pbuffer->get_field_as_num<int>(SECOND_PLACE_FINISHER_WEIGHT + _index);
            }

            inline int third_place_finisher_weight() const
            {
                return _pbuffer->get_field_as_num<int>(THIRD_PLACE_FINISHER_WEIGHT + _index);
            }

            inline std::string extra_comment_line() const
            {
                return _pbuffer->get_field_as_str(EXTRA_COMMENT_LINE + _index);
            }

            inline int weight() const
            {
                return _pbuffer->get_field_as_num<int>(WEIGHT + _index);
            }

            inline double odds() const
            {
                return _pbuffer->get_field_as_num<double>(ODDS + _index);
            }

            inline std::string race_classification() const
            {
                return _pbuffer->get_field_as_str(RACE_CLASSIFICATION + _index);
            }

            inline int claiming_price() const
            {
                return _pbuffer->get_field_as_num<int>(CLAIMING_PRICE + _index);
            }

            inline int start_call_position() const
            {
                return _pbuffer->get_field_as_num<int>(START_CALL_POSITION + _index);
            }

            inline int first_call_position() const
            {
                return _pbuffer->get_field_as_num<int>(FIRST_CALL_POSITION + _index);
            }

            inline int second_call_position() const
            {
                return _pbuffer->get_field_as_num<int>(SECOND_CALL_POSITION + _index);
            }

            inline int stretch_position() const
            {
                return _pbuffer->get_field_as_num<int>(STRETCH_POSITION + _index);
            }

            inline int finish_position() const
            {
                return _pbuffer->get_field_as_num<int>(FINISH_POSITION + _index);
            }

            inline int money_position() const
            {
                return _pbuffer->get_field_as_num<int>(MONEY_POSITION + _index);
            }

            inline std::string first_call_lengths() const
            {
                return _pbuffer->get_field_as_str(FIRST_CALL_LENGTHS + _index);
            }

            inline double second_call_lengths() const
            {
                return _pbuffer->get_field_as_num<double>(SECOND_CALL_LENGTHS + _index);
            }

            inline double stretch_lengths() const
            {
                return _pbuffer->get_field_as_num<double>(STRETCH_LENGTHS + _index);
            }

            inline double finish_lengths() const
            {
                return _pbuffer->get_field_as_num<double>(FINISH_LENGTHS + _index);
            }

            inline int bris_2_furlong_pace_figure() const
            {
                return _pbuffer->get_field_as_num<int>(BRIS_2_FURLONG_PACE_FIGURE + _index);
            }

            inline int bris_4_furlong_pace_figure() const
            {
                return _pbuffer->get_field_as_num<int>(BRIS_4_FURLONG_PACE_FIGURE + _index);
            }

            inline std::string bris_race_shape_first_call() const
            {
                return _pbuffer->get_field_as_str(BRIS_RACE_SHAPE_FIRST_CALL + _index);
            }

            inline std::string bris_race_shape_second_call() const
            {
                return _pbuffer->get_field_as_str(BRIS_RACE_SHAPE_SECOND_CALL + _index);
            }

            inline std::string bris_late_pace_figure() const
            {
                return _pbuffer->get_field_as_str(BRIS_LATE_PACE_FIGURE + _index);
            }

            inline int bris_race_rating() const
            {
                return _pbuffer->get_field_as_num<int>(BRIS_RACE_RATING + _index);
            }

            inline int bris_class_rating() const
            {
                return _pbuffer->get_field_as_num<int>(BRIS_CLASS_RATING + _index);
            }

            inline int bris_speed_rating() const
            {
                return _pbuffer->get_field_as_num<int>(BRIS_SPEED_RATING + _index);
            }

            inline int track_variant() const
            {
                return _pbuffer->get_field_as_num<int>(TRACK_VARIANT + _index);
            }

            inline double two_fulrongs_fraction() const
            {
                return _pbuffer->get_field_as_num<double>(TWO_FULRONGS_FRACTION + _index);
            }

            inline double three_fulrongs_fraction() const
            {
                return _pbuffer->get_field_as_num<double>(THREE_FULRONGS_FRACTION + _index);
            }

            inline double four_fulrongs_fraction() const
            {
                return _pbuffer->get_field_as_num<double>(FOUR_FULRONGS_FRACTION + _index);
            }

            inline double five_fulrongs_fraction() const
            {
                return _pbuffer->get_field_as_num<double>(FIVE_FULRONGS_FRACTION + _index);
            }

            inline double six_fulrongs_fraction() const
            {
                return _pbuffer->get_field_as_num<double>(SIX_FULRONGS_FRACTION + _index);
            }

            inline double seven_fulrongs_fraction() const
            {
                return _pbuffer->get_field_as_num<double>(SEVEN_FULRONGS_FRACTION + _index);
            }

            inline double eight_fulrongs_fraction() const
            {
                return _pbuffer->get_field_as_num<double>(EIGHT_FULRONGS_FRACTION + _index);
            }

            inline double ten_fulrongs_fraction() const
            {
                return _pbuffer->get_field_as_num<double>(TEN_FULRONGS_FRACTION + _index);
            }

            inline double twelve_fulrongs_fraction() const
            {
                return _pbuffer->get_field_as_num<double>(TWELVE_FULRONGS_FRACTION + _index);
            }

            inline double forteen_fulrongs_fraction() const
            {
                return _pbuffer->get_field_as_num<double>(FORTEEN_FULRONGS_FRACTION + _index);
            }

            inline double sixteen_fulrongs_fraction() const
            {
                return _pbuffer->get_field_as_num<double>(SIXTEEN_FULRONGS_FRACTION + _index);
            }

            inline double first_fraction() const
            {
                return _pbuffer->get_field_as_num<double>(FIRST_FRACTION + _index);
            }

            inline double second_fraction() const
            {
                return _pbuffer->get_field_as_num<double>(SECOND_FRACTION + _index);
            }

            inline double third_fraction() const
            {
                return _pbuffer->get_field_as_num<double>(THIRD_FRACTION + _index);
            }

            inline std::string age_sex_restrictions() const
            {
                return _pbuffer->get_field_as_str(AGE_SEX_RESTRICTIONS + _index);
            }

            inline std::string state_bred_flag() const
            {
                return _pbuffer->get_field_as_str(STATE_BRED_FLAG + _index);
            }

            inline std::string favorite_indicator() const
            {
                return _pbuffer->get_field_as_str(FAVORITE_INDICATOR + _index);
            }

            inline double final_time() const
            {
                return _pbuffer->get_field_as_num<double>(FINAL_TIME + _index);
            }

            inline std::string jockey() const
            {
                return _pbuffer->get_field_as_str(JOCKEY + _index);
            }

            inline std::string race_type() const
            {
                return _pbuffer->get_field_as_str(RACE_TYPE + _index);
            }

            inline int max_claiming_price_of_the_race() const
            {
                return _pbuffer->get_field_as_num<int>(MAX_CLAIMING_PRICE_OF_THE_RACE + _index);
            }
    private:
        parsable* _pbuffer;
        int _index;
        enum PP_FIELD_INDEX
        {
            DATE = 255,
            DAYS_SINCE_LAST = 265,
            RACE_TRACK = 275,
            RACE_NUMBER = 295,
            TRACK_CONDITION = 305,
            DISTANCE = 315,
            SURFACE = 325,
            NUMBER_OF_ENTRANTS = 345,
            POST_POSITION = 355,
            EQUIPMENT = 365,
            MEDICATION = 385,
            TRIP_COMMENT = 396,
            WINNERS_NAME = 405,
            SECOND_PLACE_FINISHER_NAME = 415,
            THIRD_PLACE_FINISHER_NAME = 425,
            WINNERS_WEIGHT = 435,
            SECOND_PLACE_FINISHER_WEIGHT = 445,
            THIRD_PLACE_FINISHER_WEIGHT = 455,
            EXTRA_COMMENT_LINE = 495,
            WEIGHT = 505,
            ODDS = 515,
            RACE_CLASSIFICATION = 535,
            CLAIMING_PRICE = 545,
            START_CALL_POSITION = 565,
            FIRST_CALL_POSITION = 575,
            SECOND_CALL_POSITION = 585,
            STRETCH_POSITION = 605,
            FINISH_POSITION = 615,
            MONEY_POSITION = 625,
            FIRST_CALL_LENGTHS = 655,
            SECOND_CALL_LENGTHS = 675,
            STRETCH_LENGTHS = 715,
            FINISH_LENGTHS = 735,
            BRIS_2_FURLONG_PACE_FIGURE = 765,
            BRIS_4_FURLONG_PACE_FIGURE = 776,
            BRIS_RACE_SHAPE_FIRST_CALL = 695,
            BRIS_RACE_SHAPE_SECOND_CALL = 755,
            BRIS_LATE_PACE_FIGURE = 815,
            BRIS_RACE_RATING = 825,
            BRIS_CLASS_RATING = 835,
            BRIS_SPEED_RATING = 845,
            TRACK_VARIANT = 865,
            TWO_FULRONGS_FRACTION = 875,
            THREE_FULRONGS_FRACTION = 885,
            FOUR_FULRONGS_FRACTION = 895,
            FIVE_FULRONGS_FRACTION = 905,
            SIX_FULRONGS_FRACTION = 915,
            SEVEN_FULRONGS_FRACTION = 925,
            EIGHT_FULRONGS_FRACTION = 935,
            TEN_FULRONGS_FRACTION = 945,
            TWELVE_FULRONGS_FRACTION = 955,
            FORTEEN_FULRONGS_FRACTION = 965,
            SIXTEEN_FULRONGS_FRACTION = 975,
            FIRST_FRACTION = 985,
            SECOND_FRACTION = 995,
            THIRD_FRACTION = 1005,
            AGE_SEX_RESTRICTIONS = 1095,
            STATE_BRED_FLAG = 1105,
            FAVORITE_INDICATOR = 1125,
            FINAL_TIME = 1035,
            JOCKEY = 1065,
            RACE_TYPE = 1085,
            MAX_CLAIMING_PRICE_OF_THE_RACE = 1211
        };
};
#endif // PASTPERFORMANCE_INCLUDED
