#pragma once

namespace InovaBee
{
class Sensor
{
public:
	Sensor();
	virtual int service(long data, int index) = 0;
};
} // namespace InovaBee