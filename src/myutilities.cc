/*******************************
 * Implementation for utility functions
 * by Ganghee Jang
 *******************************
 */

#include "myutilities.hh"

std::pair<std::size_t, std::size_t> findFieldData(std::size_t start, std::string& linebuffer) {
  std::size_t beginField = 0;
  std::size_t endField = 0;

  for (size_t iter = start; iter < linebuffer.length(); iter++) {
  	if (linebuffer.at(iter) == '\"') {
		beginField = iter + 1;
		break;
  	}
  } // we iterate through the field name. now it is time to find start point of data.

  assert(beginField != 0);

  for (size_t iter = beginField; iter < linebuffer.length(); iter++) {
  	if (linebuffer.at(iter) == '\"') {
		beginField = iter + 1;
		break;
  	}
  } // now we find start of the data

  for (size_t iter = beginField; iter < linebuffer.length(); iter++) {
  	if (linebuffer.at(iter) == '\"') {
		endField= iter - 1;
		break;
  	}
  }

  assert(endField != 0);

  return std::make_pair(beginField, endField);
}


std::string findField(std::string& fieldName, std::string& linebuffer) {

  std::size_t found = linebuffer.find(fieldName);
  assert(found != std::string::npos);

  std::pair<std::size_t, std::size_t> fieldData;
  fieldData = findFieldData( found, linebuffer );

  std::string result(linebuffer, fieldData.first, fieldData.second - fieldData.first + 1);

  return result;
}


// below multiplier is for the purpose of distiction of time stamp, not for accurate representation of time.
// so, a month's lenth is assumed as the same of 31 days.

int time_component_multiplier[] = {31622400, 2678400, 86400, 3600, 60, 1};

uint64_t timeToSec(std::string& created_time){
  assert(created_time.length() != 0);
  std::string time_formatted = created_time;

  // num_time_components, and time_component_multiplier are declared in the header file.

  std::size_t pos_next_num = 0;
  uint64_t time_in_sec = 0;

  for(int i = 0; i < NUM_TIME_COMPONENTS; i++) {
  	time_in_sec = time_in_sec + std::stoi(time_formatted, &pos_next_num) * time_component_multiplier[i];
	assert(pos_next_num < time_formatted.length());
	time_formatted = time_formatted.substr(pos_next_num + 1);
  }

  return time_in_sec;
}

uint64_t timeToSec_v2(std::string& created_time){
  assert(created_time.length() != 0);
  std::string time_formatted = created_time;

  // num_time_components, and time_component_multiplier are declared in the header file.

  std::size_t pos_next_num = 0;
  uint64_t time_in_sec = 0;

  long long year;
  unsigned int month;
  unsigned int day;
  

  for(int i = 0; i < NUM_TIME_COMPONENTS; i++) {
  	if (i == 0) { // for year
  	  year = std::stoi(time_formatted, &pos_next_num);
	  assert(pos_next_num < time_formatted.length());
	  time_formatted = time_formatted.substr(pos_next_num + 1);
	} else if (i ==1 ) { // for month
	  month = std::stoi(time_formatted, &pos_next_num);
	  assert(pos_next_num < time_formatted.length());
	  time_formatted = time_formatted.substr(pos_next_num + 1);
	} else if (i == 2) { // for day
	  day = std::stoi(time_formatted, &pos_next_num);
	  assert(pos_next_num < time_formatted.length());
	  time_formatted = time_formatted.substr(pos_next_num + 1);
	} else {
  	  time_in_sec = time_in_sec + std::stoi(time_formatted, &pos_next_num) * time_component_multiplier[i];
	  assert(pos_next_num < time_formatted.length());
	  time_formatted = time_formatted.substr(pos_next_num + 1);
	}
  }
  time_in_sec = time_in_sec + days_from_civil(year, month, day) * 86400;

  return time_in_sec;
}

long long days_from_civil(long long y, unsigned m, unsigned d) {
    y -= m <= 2;
    const long long era = (y >= 0 ? y : y-399) / 400;
    const unsigned yoe = static_cast<unsigned>(y - era * 400);      // [0, 399]
    const unsigned doy = (153*(m + (m > 2 ? -3 : 9)) + 2)/5 + d-1;  // [0, 365]
    const unsigned doe = yoe * 365 + yoe/4 - yoe/100 + doy;         // [0, 146096]
    return era * 146097 + static_cast<long long>(doe) - 719468;
}

int seconds_until_month[] = {0, 2678400, 5184000, 7862400, 10454400, 13132800, 15724800, 18403200, 21081600, 23673600, 26352000, 28944000}; 
uint64_t timeToSec_v3(std::string& created_time){
  assert(created_time.length() != 0);
  std::string time_formatted = created_time;

  // num_time_components, and time_component_multiplier are declared in the header file.

  std::size_t pos_next_num = 0;
  uint64_t time_in_sec = 0;

  for(int i = 0; i < NUM_TIME_COMPONENTS; i++) {
  	if (i==1) {
	  time_in_sec = time_in_sec + seconds_until_month[std::stoi(time_formatted, &pos_next_num)-1];
	  assert(pos_next_num < time_formatted.length());
	  time_formatted = time_formatted.substr(pos_next_num + 1);
  	} else {
  	  uint64_t temp = std::stoi(time_formatted, &pos_next_num);
	  if (i == 2) temp--; 
  	  time_in_sec = time_in_sec + temp * time_component_multiplier[i];
	  assert(pos_next_num < time_formatted.length());
	  time_formatted = time_formatted.substr(pos_next_num + 1);
  	}
  }

  return time_in_sec;
}

