/*
 * Example that uses an analog input as the sensor. But you can implement it
 * any way you like
 */
#include <Arduino.h>
#include <stdint.h>
#include <SensorMonitor.h>

// Define 2 sensor ids we'll use
const uint8_t SENSOR1 = 1;
const uint8_t SENSOR2 = 2;

// Arduino analog pins to use
const uint8_t SENSOR1_PIN = A1;
const uint8_t SENSOR2_PIN = A2;

// Define these 2 methods which will be implemented below
float getReading(uint8_t sensorId);
void onUpdate(uint8_t sensorId, float value);

// Create a Sensor Monitor and reference the functions it must use
SensorMonitor sensorMonitor(&getReading, &onUpdate);

void setup(void){
	Serial.begin(9600);
	Serial.println(F("Loading..."));

	// Register the sensors
	sensorMonitor.registerSensor(SENSOR1);
	sensorMonitor.registerSensor(SENSOR2);
	
	// Initialize sensor monitor
	sensorMonitor.begin();
}

void loop(void){
	// Call this regularly to keep the timers running
	sensorMonitor.update();
}

/**
 * The Sensor Monitor will call this method when required
 * to get an update on the reading of the specified sensor.
 */
float getReading(uint8_t sensorId){
	Serial.print(F("Get Reading: Sensor="));Serial.println(sensorId);
	if(sensorId == SENSOR1){
		return analogRead(SENSOR1_PIN);
	}
	else if(sensorId == SENSOR2){
		return analogRead(SENSOR2_PIN);
	}
	else{
		return SM_NO_READING; // Invalid sensor
	}
}

/**
 * This Sensor Monitor will call this function when the
 * value changed enough that it should be noted.
 *
 * Here you would typically send the value to a server,
 * or update a display etc.
 */
void onUpdate(uint8_t sensorId, float value){
	Serial.print(F("Sensor="));
	Serial.print(sensorId);
	Serial.print(F(" , value="));
	Serial.println(value);
}
