#include "TickPool.h"
#include "assert.h"

double fast_atof (const char *p);

TickPool TickPool::_the_singleton;

TickPool& TickPool::singleton()
{
    return _the_singleton;
}

int TickPool::size() const
{
    return _size;
}

const Tick& TickPool::operator[](int index) const
{
    assert(index>= 0 && index <_size);
    return _pool[index];
}

void TickPool::load(const std::string& filename, CONST_DATE_REF from_date, CONST_DATE_REF to_date)
{
    using namespace std;

    if(!to_date.is_not_a_date() && !from_date.is_not_a_date())
    {
        assert(to_date > from_date);
    }

    FILE* f = fopen (filename.c_str(), "r");
    if (f == NULL) 
    {
        LOG << "failed to open file: " << filename << EOL;
        throw("Error opening file");
    }

    char psz[BUFFER_LENGTH]; 
    CLEAR_BUFFER(psz)    
    int day, month, year, hour, minute, second;
    double bid, ask;
    int bytes_read;
    _size = 0;
    bool within_desired_period = from_date.is_not_a_date();

    while ( (bytes_read =fread ( psz, LINE_LENGTH, BUFFER_SIZE, f)) > 0) 
    {
        for(register int i = 0; i < bytes_read; ++i)
        {
            if(psz[i] == '\n') 
                psz[i] = '\0';  
        }

        bool exceeded_to_day = false;
        
        for(register int i = 0; i < bytes_read; ++i)
        {
            parse_tick((char*) &psz[i*LINE_LENGTH]);
            Tick& current_tick = _pool[_size];

            if(!within_desired_period && current_tick.timestamp().date() >=  from_date)
            {
                within_desired_period = true;
            }

            if(within_desired_period)
            {
                ++_size;
            }

            if(_size >= MAX_SIZE_OF_TICK_POOL)
            {
                exceeded_to_day = true;
                break;
            }

            if(!to_date.is_not_a_date() &&  current_tick.timestamp().date() > to_date)
            {
                --_size;
                exceeded_to_day = true;
                break;
            }
        }
        CLEAR_BUFFER(psz)

        if (exceeded_to_day)
        {
            break;
        }
    }      
    fclose (f);
}

void TickPool::parse_tick(char* psz)
{
    assert(_size < MAX_SIZE_OF_TICK_POOL);

    using namespace std;
    psz[2] = '\0';
    psz[5] = '\0';
    psz[8] = '\0';
    psz[11] = '\0';
    psz[14] = '\0';
    psz[17] = '\0';
    psz[25] = '\0';
    
    const int day =  atoi(psz);
    const int month = atoi((char*)&psz[3]);
    const int year = atoi((char*)&psz[6]) + 2000;
    const int hour = atoi((char*)&psz[9]);
    const int minute = atoi((char*)&psz[12]);
    const int second = atoi((char*)&psz[15]);
    const double bid = fast_atof((char*)&psz[18]);
    const double ask = fast_atof((char*)&psz[26]);
    _pool[_size].assign_values(day,month,year,hour,minute,second,bid,ask);
}


double fast_atof (const char *p)
{
    int frac;
    double sign, value, scale;
    while (white_space(*p) ) 
    {
        p += 1;
    }
    sign = 1.0;
    if (*p == '-') 
    {
        sign = -1.0;
        p += 1;
    } 
    else if (*p == '+') 
    {
        p += 1;
    }
    for (value = 0.0; valid_digit(*p); p += 1) 
    {
        value = value * 10.0 + (*p - '0');
    }
    if (*p == '.') 
    {
        double pow10 = 10.0;
        p += 1;
        while (valid_digit(*p)) 
        {
            value += (*p - '0') / pow10;
            pow10 *= 10.0;
            p += 1;
        }
    }

    frac = 0;
    scale = 1.0;
    if ((*p == 'e') || (*p == 'E')) {
        unsigned int expon;
        p += 1;
        if (*p == '-') {
            frac = 1;
            p += 1;

        } else if (*p == '+') {
            p += 1;
        }

        for (expon = 0; valid_digit(*p); p += 1) {
            expon = expon * 10 + (*p - '0');
        }
        if (expon > 308) expon = 308;
        while (expon >= 50) { scale *= 1E50; expon -= 50; }
        while (expon >=  8) { scale *= 1E8;  expon -=  8; }
        while (expon >   0) { scale *= 10.0; expon -=  1; }
    }
    return sign * (frac ? (value / scale) : (value * scale));
}
