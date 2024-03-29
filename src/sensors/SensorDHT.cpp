#include "SensorDHT.h"

// #define DHTTYPE    DHT11     // DHT 11
#define DHTTYPE DHT22 // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

namespace InovaBee
{
	SensorDHT::SensorDHT(int pin) : dht(pin, DHTTYPE)
	{
		dht.begin();

#if DEBUG
		sensor_t sensor;
		dht.temperature().getSensor(&sensor);
		Serial.println(F("------------------------------------"));
		Serial.println(F("Temperature Sensor"));
		Serial.print(F("Sensor Type: "));
		Serial.println(sensor.name);
		Serial.print(F("Driver Ver:  "));
		Serial.println(sensor.version);
		Serial.print(F("Unique ID:   "));
		Serial.println(sensor.sensor_id);
		Serial.print(F("Max Value:   "));
		Serial.print(sensor.max_value);
		Serial.println(F("°C"));
		Serial.print(F("Min Value:   "));
		Serial.print(sensor.min_value);
		Serial.println(F("°C"));
		Serial.print(F("Resolution:  "));
		Serial.print(sensor.resolution);
		Serial.println(F("°C"));
		Serial.println(F("------------------------------------"));
		// Print humidity sensor details.
		dht.humidity().getSensor(&sensor);
		Serial.println(F("Humidity Sensor"));
		Serial.print(F("Sensor Type: "));
		Serial.println(sensor.name);
		Serial.print(F("Driver Ver:  "));
		Serial.println(sensor.version);
		Serial.print(F("Unique ID:   "));
		Serial.println(sensor.sensor_id);
		Serial.print(F("Max Value:   "));
		Serial.print(sensor.max_value);
		Serial.println(F("%"));
		Serial.print(F("Min Value:   "));
		Serial.print(sensor.min_value);
		Serial.println(F("%"));
		Serial.print(F("Resolution:  "));
		Serial.print(sensor.resolution);
		Serial.println(F("%"));
		Serial.println(F("------------------------------------"));
#endif
	}

	int SensorDHT::devices()
	{
		return 2;
	}

	bool SensorDHT::service(int index, int &value)
	{
		sensors_event_t event;
		if (index == 0)
		{
			dht.temperature().getEvent(&event);

			if (isnan(event.temperature))
			{
#if DEBUG
				Serial.println(F("Error reading temperature!"));
#endif
			}
			else
			{
#if DEBUG
				Serial.print(F("Temperature: "));
				Serial.print(event.temperature);
				Serial.println(F("°C"));
#endif

				value = event.temperature;
				return true;
			}
		}

		if (index == 1)
		{
			// Get humidity event and print its value.
			dht.humidity().getEvent(&event);

			if (isnan(event.relative_humidity))
			{
#if DEBUG
				Serial.println(F("Error reading humidity!"));
#endif
			}
			else
			{
#if DEBUG
				Serial.print(F("Humidity: "));
				Serial.print(event.relative_humidity);
				Serial.println(F("%"));
#endif

				value = event.relative_humidity;
				return true;
			}
		}

		value = 0;
		return false;
	}

} // namespace InovaBee