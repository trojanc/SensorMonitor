# SensorMonitor
Arduino Sensor monitor library. This library allows you to get updates from your sensors in a controlled manner. You do not have to keep track of timeouts and deltas to determine if you need to publish an update for a sensor. All you have to do is register sensor IDs with the library, implement the method to get a reading for the sensor, and do something with the value when you are asked to.

## Installation
### Arduino IDE
Download the source code and place inside Arduino libraries folder

## Platformio
See documentation at [Platformio - Sensor Monitor](http://platformio.org/lib/show/1473/SensorMonitor/installation)

## Example use case
Let's say you have temperature sensor, and you publish the readings to a remote server when the value changes. To save power you don't want to send the value every single time if the value stayed the same. If the value changed - lets say by 2 degrees - then you want to imediately send an update to the server. If value didn't change, then you only want to update the server every 5 minutes, basically just to say the sensor is still alive. This can be done without this library, but then you'd have to keep track of timers, last value, delta value etc.

By implementing a `getReading` function which will be called for you when required, you only have to take care of implementing a way to get the value.

By implementing a `onUpdate` function you can send the value to your remote server, and it will be called only if the delta was big enough, or a long enough time has passed to do an update.

## Short summary of code use
```c
float getReading(uint8_t sensorId){
	 // Get a reading
}

void onUpdate(uint8_t sensorId, float value){
	// Do something with a reading
}

// Create a Sensor Monitor and reference the functions it must use
SensorMonitor sensorMonitor(&getReading, &onUpdate);

void setup(void){
	sensorMonitor.registerSensor(SENSOR1);
}

void loop(void){
	sensorMonitor.update();
}
```

## Configuration
The following configuration can be done by overriding the `#define` values from `SensorMonitor.h`.

### SM_RANDOM_ANALOG
**Default:** 0<br>
Analog pin to use for a random value. The random value is used to spread the sensors apart so that not all of them are queried at the exact same time.

### SM_UPDATE_INTERVAL
**Default:** 15000<br>
Millisecond interval at which all sensors should at least send an update

### SM_UPDATE_MIN_DELAY
**Default:** 1500<br>
Minimum amount to delay before taking readings

### SM_VALUE_DELTA
**Default:** 0.5<br>
Delta required before updating immediately

### SM_VALUE_DELTA
**Default:** 5<br>
Maximum number of sensors that can be monitored.
Keep this as small as required to save memory
