#ifndef PRESSURE_SENSOR_HPP
#define PRESSURE_SENSOR_HPP


using namespace std
#include <SparkFun_MS5803_I2C.h>
#include <Wire.h>

class PressureSensor(){

private:

	int abs_pressure, rel_pressure;
	int elevation; 


public:
	PressureSensor();
	~PressureSensor()

	static const unsigned int BASE_ALT = 329 // Altitude in waterloo
	int get_abs_pressure();
	int get_abs_pressure();
	int get_current_elevation();
	void sea_level(int abs_pressure);
}

