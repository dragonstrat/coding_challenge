/*******************************
 * header for utility functions
 * by Ganghee Jang
 *******************************
 */

#ifndef __MY_UTILITY_FUNCTIONS__
#define __MY_UTILITY_FUNCTIONS__

#include <string>
#include <assert.h>
#include <utility>
#include <cstdint>
#include <iostream>
#include <type_traits>
#include <exception>



// NUM_TIME_COMPONENTS is the number of fields in the created_time field
#define NUM_TIME_COMPONENTS 6
#define WINDOW_SIZE 60


// below array is to keep the multipliers to convert each field into seconds.
// definition can be found at myutilities.cc
extern int time_component_multiplier[];
extern int seconds_until_month[];

std::pair<std::size_t, std::size_t> findFieldData(std::size_t start, std::string& linebuffer);
std::string findField(std::string& fieldName, std::string& linebuffer);
uint64_t timeToSec(std::string& created_time);

uint64_t timeToSec_v2(std::string& created_time);

uint64_t timeToSec_v3(std::string& created_time);


// the following code is from Internet search.
long long days_from_civil(long long y, unsigned m, unsigned d);

#endif
