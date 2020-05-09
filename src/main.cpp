#include "Arduino.h"

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

// Sensors
#include <sensors/Sensor.h>
#include <sensors/SensorDallas.h>
#include <sensors/SensorDHT.h>

#define ONE_WIRE 22
#define DHT_PIN 17

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 915E6



// Sensors

OneWire oneWire(ONE_WIRE);

std::vector<InovaBee::Sensor *> sensors;

void setup()
{
#if DEBUG
	Serial.begin(115200);
	Serial.println("InovaBee");
#endif

	sensors.push_back(new InovaBee::SensorDallas(&oneWire));
	sensors.push_back(new InovaBee::SensorDHT(DHT_PIN));

	}

	//SPI LoRa pins
	SPI.begin(SCK, MISO, MOSI, SS);
	//setup LoRa transceiver module
	LoRa.setPins(SS, RST, DIO0);

	if (!LoRa.begin(BAND))
	{
		Serial.println("Starting LoRa failed!");
		while (1)
			;
	}
	Serial.println("LoRa Initializing OK!");
	delay(2000);
}

void serviceSensor()
{
#if DEBUG
	Serial.print("Current time: ");
	Serial.println(millis());
#endif

	for (unsigned int i = 0; i < sensors.size(); ++i)
	{
		sensors.at(i)->service(0, 0);
	}
}

void loop()
{
	serviceSensor();

	// Serial.print("Sending packet: ");
	// Serial.println(counter);

	delay(10000);
}