#pragma once

namespace InovaBee
{
class Sensor
{
public:
	Sensor();
	virtual bool service(int index, int &value) = 0;
	virtual int devices() = 0;
};
} // namespace InovaBee