#include "SensorDallas.h"

namespace InovaBee
{
SensorDallas::SensorDallas(OneWire *onewire) : manager(onewire)
{
	sensor_count = 0;
}

int SensorDallas::service(long data, int index)
{
	if (sensor_count == 0)
	{
#if DEBUG
		Serial.println("[DALLAS] No sensors found, searching again...");
#endif

		manager.begin();
		sensor_count = manager.getDeviceCount();
		manager.setResolution(9);
	}

	manager.requestTemperatures();

	for (int i = 0; i < sensor_count; ++i)
	{
		long temperature = (long)manager.getTempCByIndex(i);

#if DEBUG
		Serial.print("Temperature ");
		Serial.print(i);
		Serial.print(": ");
		Serial.println(temperature);
#endif
	}

	return 0;
}
} // namespace InovaBee