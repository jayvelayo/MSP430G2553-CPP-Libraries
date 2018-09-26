/**
 * SerialLibrary.h
 *
 */

#ifndef SerialLibrary_H_
#define SerialLibrary_H_

#include <msp430.h>

#define TRUE true
#define FALSE false
#define MAX_BUFF_SIZE 64

class CircularBuffer {
private:
	char charBuffer[MAX_BUFF_SIZE];
	unsigned int startIndex;
	unsigned int stopIndex;
	bool isFull;

public:
	CircularBuffer();
	bool enqueue(char newByte);
	char tryDequeue(void);
	int getBufferLength(void);
	bool isEmpty();
};

class SerialPort {
public:
	CircularBuffer circBuffer;
	SerialPort();
	void txChar(char c);
	void begin();
private:

};

#endif /* SerialLibrary_H_ */
