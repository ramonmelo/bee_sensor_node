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
		bool service(int index, int &value);
		int devices();
	private:
		OneWire oneWire;
		DallasTemperature manager;
		int sensor_count;
};
} // namespace InovaBee