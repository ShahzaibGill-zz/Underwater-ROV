#include <pressure_sensor.h>

void setup() {
    Serial.begin(115200);
    //Retrieve calibration constants for conversion math.
    pressure_sensor.reset();
    pressure_sensor.begin();
    pressure_baseline = pressure_sensor.getPressure(ADC_4096);
}
