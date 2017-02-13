/**
MIT License

Copyright (c) 2017 TrojanC

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
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
		// Check if we may ask this sensor for a reading
		if(SensorMonitor::hasTimedout(_monitoredSensors[i].last_reading, SM_UPDATE_MIN_DELAY)){

			// Update the last time that we took a reading from this sensor
			_monitoredSensors[i].last_reading = millis();

			// Call the configured function to get a reading for the sensor
			float newValue = _getReadingCallback(_monitoredSensors[i].sensorId);

			// The sensor could not produce a reading at this moment
			if(newValue == SM_NO_READING){
				continue; // Continue to the next sensor
			}

			// Check if the value delta is big enough to cause an immediate update
			bool reachedDelta = SensorMonitor::checkDelta(_monitoredSensors[i].last_value, newValue);

			// If we need to update immediately, or if the last time we updated was long enough ago
			if(reachedDelta || SensorMonitor::hasTimedout(_monitoredSensors[i].last_update, SM_UPDATE_INTERVAL)){

				// Save the last value that we called an update for
				_monitoredSensors[i].last_value = newValue;

				// Call the configured update function
				_onUpdateCallback(_monitoredSensors[i].sensorId, _monitoredSensors[i].last_value);

				// Save the time we last did an update call
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
