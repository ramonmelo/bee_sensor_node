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

#define SEND_DELAY 1000 * 60 // 60 secs

// Sensors

byte *buffer;
size_t length = 0;
OneWire oneWire(ONE_WIRE);
std::vector<InovaBee::Sensor *> sensors;

void sendData(byte *data, int size)
{
	//Send LoRa packet to receiver
	if (LoRa.beginPacket())
	{
		LoRa.write(data, size);
		LoRa.endPacket();
	}
}

void setup()
{
#if DEBUG
	Serial.begin(115200);
	Serial.println("InovaBee");
#endif

	sensors.push_back(new InovaBee::SensorDallas(&oneWire));
	sensors.push_back(new InovaBee::SensorDHT(DHT_PIN));

	// Init buffer
	for (unsigned int i = 0; i < sensors.size(); ++i)
	{
		length += sensors.at(i)->devices();
	}
	buffer = new byte[length];

#if DEBUG
	Serial.print("Starting buffer with length: ");
	Serial.println(length);
#endif

	//SPI LoRa pins
	SPI.begin(SCK, MISO, MOSI, SS);
	//setup LoRa transceiver module
	LoRa.setPins(SS, RST, DIO0);

	if (!LoRa.begin(BAND))
	{
#if DEBUG
		Serial.println("Starting LoRa failed!");
#endif
		while (1)
		{
			// do nothing
		}
	}
#if DEBUG
	Serial.println("LoRa Initializing OK!");
#endif

	delay(2000);
}

void write(byte *buffer, int i, int value)
{
	if (value > 127)
	{
		value = 127;
	}
	else if (value < -127)
	{
		value = -127;
	}

	byte signal = value <= 0 ? 0x80 : 0x00; // if negative, signal on the last bit

	if (value <= 0) // if negative
	{
		value = value * -1; // invert signal
	}

	buffer[i] = signal | value;

#if DEBUG
	Serial.print("Write: ");
	Serial.println(buffer[i]);
#endif
}

void serviceSensor()
{
#if DEBUG
	Serial.print("Current time: ");
	Serial.println(millis());
#endif

	// Clear buffer
	for (size_t i = 0; i < length; i++)
	{
		buffer[i] = 0;
	}

	// Write data
	int position = 0;
	for (size_t i = 0; i < sensors.size(); ++i)
	{
		int devices = sensors.at(i)->devices();

		for (size_t j = 0; j < devices; j++)
		{
			int value = sensors.at(i)->service(j); // get value from sensor

			write(buffer, position, value); // write to buffer
			position++;						// advance current position
		}
	}

	// Send data
	sendData(buffer, length);
}

void fakeSensor()
{
	byte *buffer = new byte[3];

	buffer[0] = 1;
	buffer[1] = 2;
	buffer[2] = 3;

	sendData(buffer, 3);
}

void loop()
{
	fakeSensor();
	// serviceSensor();
	delay(SEND_DELAY);
}