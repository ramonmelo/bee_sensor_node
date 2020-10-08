#include "SensorDallas.h"

namespace InovaBee
{
	SensorDallas::SensorDallas(OneWire *onewire) : manager(onewire)
	{
		sensor_count = 0;
	}

	int SensorDallas::devices()
	{
		return 1;
	}

	bool SensorDallas::service(int index, int &value)
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

		if (sensor_count > 0)
		{
			int temperature = manager.getTempCByIndex(0);

#if DEBUG
			Serial.print("Temperature: ");
			Serial.println(temperature);
#endif

			value = temperature;
			return true;
		}

		value = 0;
		return false;
	}
} // namespace InovaBee