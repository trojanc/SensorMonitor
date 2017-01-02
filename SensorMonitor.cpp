/*
 * SensorMonitor.cpp
 *
 *  Created on: Jan 1, 2017
 *      Author: Charl-PC
 */
#include "Arduino.h"
#include "SensorMonitor.h"

SensorMonitor::SensorMonitor(SensorMonitorReading getReadingCallback, SensorMonitorUpdate onUpdateCallback){
	_onUpdateCallback = onUpdateCallback;
	_getReadingCallback = getReadingCallback;
}

void SensorMonitor::registerSensor(const uint8_t sensorId){
	_monitoredSensors[_items++] = {
		sensorId,			// Sensor id
		(unsigned long)0, 	// Last reading
		(unsigned long)0, 	// Last update
		(float)0			// Last value
	};
}

void SensorMonitor::begin(void){
	randomSeed(analogRead(SM_RANDOM_ANALOG));
	for(int i = 0; i < _items ; i++){
		_monitoredSensors[i].last_reading = random(SM_UPDATE_MIN_DELAY);
	}
}

void SensorMonitor::update(void){
	for(int i = 0; i < _items ; i++){
		if(SensorMonitor::hasTimedout(_monitoredSensors[i].last_reading, SM_UPDATE_MIN_DELAY)){
			_monitoredSensors[i].last_reading = millis();
			float newValue = _getReadingCallback(_monitoredSensors[i].sensorId);
			bool reachedDelta = SensorMonitor::checkDelta(_monitoredSensors[i].last_value, newValue);
			if(reachedDelta || SensorMonitor::hasTimedout(_monitoredSensors[i].last_update, SM_UPDATE_INTERVAL)){
				_monitoredSensors[i].last_value = newValue;
				_onUpdateCallback(_monitoredSensors[i].sensorId, _monitoredSensors[i].last_value);
				_monitoredSensors[i].last_update = millis() + random(SM_UPDATE_MIN_DELAY / 3); // set next update to a random bit ahead in time
			}
		}
	}
}

bool SensorMonitor::checkDelta(const float oldValue, const float newValue){
	float diff = oldValue - newValue;
	diff = abs(diff);
	return diff >= (float)SM_VALUE_DELTA;
}

bool SensorMonitor::hasTimedout(const uint32_t from, const uint32_t period){
	return ((unsigned long)(millis() - from)) >= period;
}
