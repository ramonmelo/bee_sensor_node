#pragma once

namespace InovaBee
{
class Sensor
{
public:
	Sensor();
	virtual int service(int index) = 0;
	virtual int devices() = 0;
};
} // namespace InovaBee