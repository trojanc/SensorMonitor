/*
 * Example that uses an input button as a sensor. This can be a PIR device, a door switch
 * or any other device that acts like a "switch".
 */
#include <Arduino.h>
#include <stdint.h>
#include <SensorMonitor.h>

// Define the ID for the door sensor
const uint8_t SENSOR_DOOR = 1;

// Arduino pin to use to detect the door
const uint8_t SENSOR_DOOR_PIN = 2;

// Define these 2 methods which will be implemented below
float getReading(uint8_t sensorId);
void onUpdate(uint8_t sensorId, float value);

// Create a Sensor Monitor and reference the functions it must use
SensorMonitor sensorMonitor(&getReading, &onUpdate);

void setup(void){
	Serial.begin(9600);
	Serial.println(F("Loading..."));

	// Configure the door pin as an input
	pinMode(SENSOR_DOOR_PIN, INPUT);

	// Register the sensor
	sensorMonitor.registerSensor(SENSOR_DOOR);
	
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
	if(sensorId == SENSOR_DOOR){
		return digitalRead(SENSOR_DOOR_PIN);
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
