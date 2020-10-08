#include "Arduino.h"

#include <WiFi.h>

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

// Sensors
#include <sensors/Sensor.h>
#include <sensors/SensorDallas.h>
#include <sensors/SensorDHT.h>

#define ONE_WIRE 13
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

#define SEND_DELAY 1000 * 2

#define SENSOR_ID "AAA0001"

#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 60	   /* Time ESP32 will go to sleep (in seconds) */

bool serviceSensor();
void sendData(byte *data, int size);
void write(byte *buffer, int i, int value);

// Sensors

byte *buffer;
size_t length = 0;
OneWire oneWire(ONE_WIRE);
std::vector<InovaBee::Sensor *> sensors;

void setup()
{
	WiFi.mode(WIFI_MODE_NULL);
	btStop();

	delay(1000);

#if DEBUG
	Serial.begin(115200);
	delay(1000); //Take some time to open up the Serial Monitor

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
		
		// If LoRa was not enabled, we restart
		ESP.restart();
	}
#if DEBUG
	Serial.println("LoRa Initializing OK!");
#endif

	int tries = 10;
	// Send Data
	while (serviceSensor() == false)
	{
		tries--;

		if (tries < 0)
		{
			break;
		}

		delay(SEND_DELAY);
	}

	// Start Deep Sleep
	ESP.deepSleep(TIME_TO_SLEEP * uS_TO_S_FACTOR);
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

bool serviceSensor()
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
			int value = 0;
			if (sensors.at(i)->service(j, value) == false)
			{
				return false;
			}

			write(buffer, position, value); // write to buffer
			position++;						// advance current position
		}
	}

	// Send data
	sendData(buffer, length);
	return true;
}

void sendData(byte *data, int size)
{
	//Send LoRa packet to receiver
	if (LoRa.beginPacket())
	{
		LoRa.print(SENSOR_ID);
		LoRa.write(data, size);
		LoRa.endPacket();

#if DEBUG
		Serial.println("Data Sent...");
#endif
	}
}

void loop()
{
	// EMPTY
}