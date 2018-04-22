/*
 * This example builds on the previous example (switch.ino), where it uses an interrupt to 
 * notify the SensorMonitor library to imediately call an update for a sensor.
 */
#include <Arduino.h>
#include <stdint.h>
#include <SensorMonitor.h>

// Define the ID for the door sensor
const uint8_t SENSOR_DOOR = 1;

// Arduino pin to use to detect the door
// This pin should have interrupts available (normally digital pin 2 and 3)
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

	// Attach a interupt listener to the pin
	attachInterrupt(digitalPinToInterrupt(SENSOR_DOOR_PIN), [](){
		// Notify the SensorMonitor to imediately call an update for the specified
		// sensor. The update will happen on the next sensorMonitor.update() call
		// You do not want to do too much work in the interupt handler!
		sensorMonitor.sheduleUpdate(SENSOR_DOOR);
	}, CHANGE);
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
