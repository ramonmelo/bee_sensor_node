#pragma once

#include "Sensor.h"
#include <OneWire.h>
#include <DallasTemperature.h>

namespace InovaBee
{
class SensorDallas : public Sensor
{
	public:
		SensorDallas(OneWire *onewire);
		int service(long data, int index);
	private:
		OneWire oneWire;
		DallasTemperature manager;
		int sensor_count;
};
} // namespace InovaBee