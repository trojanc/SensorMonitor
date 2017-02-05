#ifndef __SENSOR_MONITOR_H__
#define __SENSOR_MONITOR_H__
#include <stdint.h>

// Analog pin to use for random seed
// This pin should remain floating
#ifndef SM_RANDOM_ANALOG
#define SM_RANDOM_ANALOG 0
#endif

// Millisecond interval at which all sensors should at least send an update
#ifndef SM_UPDATE_INTERVAL
#define SM_UPDATE_INTERVAL 15000
#endif

// Minimum amount to delay before taking readings
#ifndef SM_UPDATE_MIN_DELAY
#define SM_UPDATE_MIN_DELAY 1500
#endif

// Delta required before updating immediately
#ifndef SM_VALUE_DELTA
#define SM_VALUE_DELTA 0.5
#endif

// Maximum number of sensors that can be monitored
// Keep this as small as required to save memory
#ifndef SM_NUM_SENSORS
#define SM_NUM_SENSORS 5
#endif


struct __attribute__((__packed__)) SensorItem{
	uint8_t sensorId;
	uint32_t last_reading;
	uint32_t last_update;
	float last_value;
};

// Handler when a sensor monitor should be updated
typedef void (*SensorMonitorUpdate)(uint8_t sensorId, float value);

// Function called to get a reading
typedef float (*SensorMonitorReading)(uint8_t sensorId);



class SensorMonitor
{
public:
	SensorMonitor(SensorMonitorReading getReadingCallback, SensorMonitorUpdate onUpdateCallback);
	void registerSensor(const uint8_t sensorId);
	void begin(void);
	void update(void);
private:
	SensorItem _monitoredSensors[SM_NUM_SENSORS];

	// Index of last inserted sensor
	uint8_t _items = 0;

	SensorMonitorUpdate _onUpdateCallback;
	SensorMonitorReading _getReadingCallback;

	bool hasTimedout(const uint32_t from, const uint32_t period);
	bool checkDelta(const float oldValue, const float newValue);
};


#endif /* __SENSOR_MONITOR_H__ */
