/*===================================================================
PMS7003 датчик пыли

https://how2electronics.com/interfacing-pms5003-air-quality-sensor-arduino/ - Допилил
The Plantower PMS5003 is a low-cost laser particle counter, one of a range of sensors by Plantower that also include the PMS1003, PMS3003,
and PMS7003

Шлейф белый   ESP12	ESP32-UNO
1 *	VCC    ---  		вишн--- 5В
2	GND    --- 		син --- 0В
3	~SLEEP --- 	D5	зел --- D21
4	TXD    --- 	Rx	жел --- D16_rx2
5	RXD    --- 	Tx	ора --- D17_tx2
6	~RST   --- 	-	кра --- D16

Note:
	PMS сам посылает данные при включении с интервалом 200-800мс и если их не вычитывать с порта то будет креш

реакция на пыль:   сильнее реагирует    *        *              не показательны
10:37:16:906 -> 0.3=1266   0.5=384   1.0=69  |2.5=6|         5.0=228         10.0=280 
10:37:39:128 -> 0.3=1782   0.5=534   1.0=163 |2.5=24|        5.0=228         10.0=283	

10:47:17:149 -> 0.3=0    0.5=0   1.0=0   |2.5=0|         5.0=232         10.0=310 закрыта щель
ESP32 - use Serial2

===================================================================*/
#include <Arduino.h>
#include "cvv_include.h"
#include "Net.h"

extern const int pinPMSset = 5; //, pinPMSrst, pinPMStx, pinPMSrx;
uint8_t buffer[32];
static int pmsStat = 0;

void pmsInit() // sleep - выкл вентилятор
{
	pinMode(pinPMSset, OUTPUT); digitalWrite(pinPMSset, LOW);
	//pinMode(pinPMSrst, OUTPUT); digitalWrite(pinPMSrst, HIGH);
	
	Serial.begin(9600);
	//esp32 Serial.begin(9600, SERIAL_8N1, pinPMStx, pinPMSrx); //rx, tx
	// Serial2.setRxBufferSize(256);
}

struct pms5003data // равен PMS7003
{ 
	uint16_t framelen;
	uint16_t pm10_standard, pm25_standard, pm100_standard;
	uint16_t pm10_env, pm25_env, pm100_env;
	uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
	uint16_t unused;
	uint16_t checksum;
};

struct pms5003data data;
boolean readPMSdata(Stream *s);

void pmsWake()
{
	digitalWrite(pinPMSset, HIGH); //sph("\r\npmsGo"); //корректные данные будут через >6c
	// TerminalP("pmsWake");
}
void pmsSleep()
{
	digitalWrite(pinPMSset, LOW); //sph("\r\npmsSleep");
	// TerminalP("pmsSleep");
}

void serialClr()
{
	// s->flush(); не работает // s->readBytes(buffer, 500); если читать больше nb - задержка
	while(Serial.available())
	{
		Serial.readBytes(buffer, 1); //sp(".");
	}
}

int pmsRead()
{
	if (!readPMSdata(&Serial)) return 0;
	// if(prn < 1) return;
	// Serial.println();
	// Serial.println("---------------------------------------");
	// Serial.println("Concentration Units (standard)");
	// Serial.print("PM 1.0: "); Serial.print(data.pm10_standard);
	// Serial.print("\t\tPM 2.5: "); Serial.print(data.pm25_standard);
	// Serial.print("\t\tPM 10: "); Serial.println(data.pm100_standard);
	// Serial.println("---------------------------------------");
	// Serial.println("Concentration Units (environmental)");
	// Serial.print("PM 1.0: "); Serial.print(data.pm10_env);
	// Serial.print("\t\tPM 2.5: "); Serial.print(data.pm25_env);
	// Serial.print("\t\tPM 10: "); Serial.println(data.pm100_env);
	// Serial.println("---------------------------------------");
	// Serial.print("Particles > 0.3um / 0.1L air:"); Serial.println(data.particles_03um);
	// Serial.print("Particles > 0.5um / 0.1L air:"); Serial.println(data.particles_05um);
	// Serial.print("Particles > 1.0um / 0.1L air:"); Serial.println(data.particles_10um);
	// Serial.print("Particles > 2.5um / 0.1L air:"); Serial.println(data.particles_25um);
	// Serial.print("Particles > 5.0um / 0.1L air:"); Serial.println(data.particles_50um);
	// Serial.print("Particles > 10.0 um / 0.1L air:"); Serial.println(data.particles_100um);

	//brief: 0.3=543  0.5=161        < 1.0=52 2.5=0 >        5.0=220  10.0=363 |
	// Serial.print("\r\n0.3="); Serial.print(data.particles_03um);
	// Serial.print("\t 0.5="); Serial.print(data.particles_05um);
	// Serial.print("\t< 1.0="); Serial.print(data.particles_10um);
	// Serial.print(" 2.5="); Serial.print(data.particles_25um);
	// Serial.print(" >\t5.0="); Serial.print(data.particles_50um);
	// Serial.print("\t 10.0="); Serial.print(data.particles_100um); sp(" | ");
	return 1;
}

int pmsProc(int interval) // 1s
{
	if(pmsStat == 0) pmsWake();
	if(++pmsStat < 20) { serialClr(); return 2; } //прогрев 20с, 10c мало - нули
	if(pmsStat > 1000) //sleep interval
	{
		serialClr();
	 	if(pmsStat > (1000 + interval - 20 - 7)) pmsStat = 0; //7-коррекция
		return 0;
	}
	if(pmsRead())
	{
		pmsSleep(); pmsStat = 1000;
		return 1;
    }
	return -1;
}

boolean readPMSdata(Stream *s)
{
	int nb = s->available();
	// TerminalP("b", nb);
	if(!nb){ return false; }
	// sph("nb1", nb); //256 байт за 10с
 	if (nb < 32) { return false; }

 	while(nb--)
	{
		if(s->peek() == 0x42) break;
		s->read(); // sp("#");
	}
	nb = s->available();
	if (nb < 32) { return false; }

    s->readBytes(buffer, 32);
	
    // Read a byte at a time until we get to the special '0x42' start-byte
    // if (s->peek() != 0x42) { s->read(); return false; }
    // Now read all 32 bytes
    // if (s->available() < 32) { return false; }

    // get checksum ready
    uint16_t sum = 0;
    for (uint8_t i=0; i<30; i++) { sum += buffer[i]; }

    // debugging: 0x42 0x4D 0x0 0x1C 0x0 0x2 0x0 0x4 0x0 0x4 0x0 0x2 0x0 0x4 0x0 0x4 0x2 0x70 0x0 0xB7 0x0 0x2C 0x0 0x2 0x0 0xF7 0x1 0x32 0x9A 0x0 0x3 0xDA 
	// Serial.println();
    // for (uint8_t i=0; i<32; i++) {
    // 	Serial.print("0x"); Serial.print(buffer[i], HEX); Serial.print(" ");
    // }
    // Serial.println();
    
    // The data comes in endian'd, this solves it so it works on all platforms
    uint16_t buffer_u16[15];
    for (uint8_t i=0; i<15; i++) {
        buffer_u16[i] = buffer[2 + i*2 + 1];
        buffer_u16[i] += (buffer[2 + i*2] << 8);
    }

    // put it into a nice struct :)
    memcpy((void *)&data, (void *)buffer_u16, 30);

    //все вычитать
	serialClr();
    
    if (sum != data.checksum) {
        // Serial.println(" PMS_ChecksumFailure");
        return false;
    }
    
    return true; // success!
}

int pmsGetData(int ch)
{
	switch(ch)
	{
		case VIRT_PMS_3:	return data.particles_03um;
		case VIRT_PMS_5:	return data.particles_05um; 
		case VIRT_PMS_10:	return data.particles_10um;
	}
	return data.particles_25um;
}