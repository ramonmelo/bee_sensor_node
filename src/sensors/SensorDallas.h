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
		int service(int index);
		int devices();
	private:
		OneWire oneWire;
		DallasTemperature manager;
		int sensor_count;
};
} // namespace InovaBee