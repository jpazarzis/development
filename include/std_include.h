// std_include.h
//
// Author        : John Pazarzis
// Creation date : Wed 09 Oct 2013 03:09:52 PM EDT
//
// Summary
//      Includes standard header files 

#ifndef STD_INCLUDE_INCLUDED
#define STD_INCLUDE_INCLUDED

#include "Object.h"
#include "Logger.h"
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>      
#include <algorithm> 
#include <vector>
#include <fstream>
#include <assert.h>
#include<limits>
#include "toolkit.h"
#include "Identifiable.h"
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace boost::posix_time; 
using namespace boost::gregorian;


using DATE = boost::gregorian::date ;
using CONST_DATE_REF = const boost::gregorian::date& ;

using DATE_TIME = boost::posix_time::ptime;
using CONST_DATE_TIME_REF = const boost::posix_time::ptime&;

using namespace boost::gregorian;
using namespace boost::posix_time;


#define INVALID_FITNESS -9999999.9

#define white_space(c) ((c) == ' ' || (c) == '\t')
#define valid_digit(c) ((c) >= '0' && (c) <= '9')

#define BUFFER_LENGTH 3500
#define LINE_LENGTH 34
#define BUFFER_SIZE 100
#define CLEAR_BUFFER(buffer) for (int i = 0; i < BUFFER_LENGTH; ++i) buffer[i] = '\0';


#endif // STD_INCLUDE_INCLUDED
