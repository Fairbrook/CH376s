#ifndef CH376s_h
#define CH376s_h

#include <Arduino.h>

class CH376s
{
	public:
		CH376s(uint8_t*, uint8_t, uint8_t, uint8_t, uint8_t);
		bool checkExist();
		bool setUSBMode(uint8_t);
		void resetAll();
		bool writeFile();
		void setFileName(String);
		bool diskConnect();
		bool diskMount();
		bool fileCreate();
		bool fileClose();
		uint8_t getStatus();

	private:
		void CMD(uint8_t);
		uint8_t read();
		void write(uint8_t);
		void portWrite(uint8_t);
		uint8_t portRead();
		uint8_t _portPins[8];
		uint8_t _RD;
		uint8_t _WR;
		uint8_t _CS;
		uint8_t _A;
};

#endif