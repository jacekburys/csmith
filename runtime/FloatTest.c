// Extension to CSmith to test floating point optimizations
// Jacek Burys, Imperial College London 2015

#include "FloatTest.h"

#ifdef FLOAT_TEST_ENABLED

///////////////////////////////////

float_interval_t add_float_interval(float_interval_t in1, float_interval_t in2){
	  float_interval_t result;
	  result.lower = 0.0f;
	  result.upper = 0.0f;
	  return result;
}

float_interval_t sub_float_interval(float_interval_t in1, float_interval_t in2){
	  float_interval_t result;
	  result.lower = 0.0f;
	  result.upper = 0.0f;
	  return result;
}

float_interval_t mul_float_interval(float_interval_t in1, float_interval_t in2){
	  float_interval_t result;
	  result.lower = 0.0f;
	  result.upper = 0.0f;
	  return result;
}

float_interval_t div_float_interval(float_interval_t in1, float_interval_t in2){
	  float_interval_t result;
	  result.lower = 0.0f;
	  result.upper = 0.0f;
	  return result;
}



///////////

float_interval_t char_to_float_interval(int8_t x){
  float_interval_t result;
  result.lower = 0.0f;
  result.upper = 0.0f;
  return result;
}

float_interval_t short_to_float_interval(int16_t x){
	  float_interval_t result;
	  result.lower = 0.0f;
	  result.upper = 0.0f;
	  return result;

}

float_interval_t int_to_float_interval(int32_t x){
	  float_interval_t result;
	  result.lower = 0.0f;
	  result.upper = 0.0f;
	  return result;
}

float_interval_t long_to_float_interval(int32_t x){
	  float_interval_t result;
	  result.lower = 0.0f;
	  result.upper = 0.0f;
	  return result;
}

float_interval_t long_long_to_float_interval(int64_t x){
	  float_interval_t result;
	  result.lower = 0.0f;
	  result.upper = 0.0f;
	  return result;
}

// unsigned

float_interval_t uchar_to_float_interval(uint8_t x){
	  float_interval_t result;
	  result.lower = 0.0f;
	  result.upper = 0.0f;
	  return result;
}

float_interval_t ushort_to_float_interval(uint16_t x){
	  float_interval_t result;
	  result.lower = 0.0f;
	  result.upper = 0.0f;
	  return result;
}

float_interval_t uint_to_float_interval(uint32_t x){
	  float_interval_t result;
	  result.lower = 0.0f;
	  result.upper = 0.0f;
	  return result;
}

float_interval_t ulong_to_float_interval(uint32_t x){
	  float_interval_t result;
	  result.lower = 0.0f;
	  result.upper = 0.0f;
	  return result;
}

float_interval_t ulong_long_to_float_interval(uint64_t x){
	  float_interval_t result;
	  result.lower = 0.0f;
	  result.upper = 0.0f;
	  return result;
}


///////////////////////////////////

#endif
