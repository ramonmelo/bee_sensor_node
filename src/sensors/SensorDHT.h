#pragma once

#include "Sensor.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

namespace InovaBee
{
class SensorDHT : public Sensor
{
public:
	SensorDHT(int pin);
	bool service(int index, int &value);
	int devices();

private:
	DHT_Unified dht;
};
} // namespace InovaBee