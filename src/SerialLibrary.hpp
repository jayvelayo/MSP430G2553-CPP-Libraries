/**
 * SerialLibrary.h
 *
 * Creates a SerialPort class that handles serial communications through UART.
 * This opens a line on P1.1 / P1.2, therefore these ports should not be used.
 */

#ifndef SerialLibrary_H_
#define SerialLibrary_H_

#define MAX_BUFF_SIZE 64

#include <string>
#include <msp430.h>

typedef struct _baud_set {
	long baud;
	unsigned short UCBRx;
	unsigned short UCBRSx;
} baud_set;

static const baud_set baud_settings[] = {
	//baud, UCBRx, 	UCBRSx
	//{9600,	104, 2},
	{9600, 1250, 0} //temp. for 12MHz
	{19200, 52, 0},
	{38400, 26, 0},
	{56000, 17, 14},
	{115200, 8, 12},
	{128000, 7, 14},
	{256000, 3, 14}
};

class CircularBuffer {
private:
	char charBuffer[MAX_BUFF_SIZE];
	unsigned int startIndex;
	unsigned int stopIndex;
	bool isFull;

public:  
	CircularBuffer();
	inline bool enqueue(char newByte);
	char tryDequeue(void);
	int bufferSize();
};

class SerialPort {

private:
	CircularBuffer circBuffer;
	static __interrupt void USCI0RX_ISR(void);
	static SerialPort* thisInstance;
public:
	SerialPort();
	void begin(long baudrate);
	void printChar(char c);
	void printLine(std::string &str);
	char readChar();
	std::string readLine();

};

#endif /* SerialLibrary_H_ */ 
