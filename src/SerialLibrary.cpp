/**
 * SerialLibrary.cpp
 *
 */

#include "SerialLibrary.hpp"

CircularBuffer::CircularBuffer() {
	startIndex = 0;
	stopIndex = 0;
	isFull = true;
}

/*
 * enqueue:
 */
bool CircularBuffer::enqueue(char newByte) {
	if(isFull) {
		return false;
	}
	else {
		charBuffer[stopIndex++]= newByte;
		if(stopIndex >= MAX_BUFF_SIZE) { stopIndex = 0; }
		if(stopIndex == startIndex) { isFull = true; }
		return true;
	}
}

char CircularBuffer::tryDequeue(void) {
	if(bufferSize() == 0) {
		return '\0';
	}
	else {
		char returnChar = charBuffer[startIndex++];
		if(startIndex>=MAX_BUFF_SIZE) {startIndex = 0;}
		if(isFull) { isFull=false; }
		return returnChar;
	}
}

int CircularBuffer::bufferSize() {
	if(isFull) return MAX_BUFF_SIZE;
	else if (stopIndex >= startIndex) return stopIndex-startIndex;
	else return (MAX_BUFF_SIZE - startIndex - stopIndex);
}



SerialPort::SerialPort() {
	thisInstance = this;
}

SerialPort* SerialPort::thisInstance = NULL;

void SerialPort::begin(long baudrate) {
	int index = 0;
	unsigned const int size = sizeof(baud_settings)/sizeof(baud_settings[0]);

	for (unsigned int i = 0; i < size; i++) {
		if (baud_settings[i].baud == baudrate) index = i;
	}

	UCA0CTL1 |= UCSSEL_2 + UCSWRST;                     // SMCLK
    UCA0BR0 = (baud_settings[index].UCBRx & 0xff);                            // 1MHz 9600
    UCA0BR1 = (baud_settings[index].UCBRx >> 8) & 0xff; ;                              // 1MHz 9600
    UCA0MCTL = baud_settings[index].UCBRSx;                        // Modulation UCBRSx = 1
    P1DIR |= BIT1+BIT2;
    P1SEL |= BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
    P1SEL2 |= BIT1 + BIT2 ;                    // P1.1 = RXD, P1.2=TXD
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
}

void SerialPort::printChar(char c) {
	while (!(IFG2&IFG2&UCA0TXIFG));
    UCA0TXBUF = c;
}

void SerialPort::printLine(std::string &str) {
	for(unsigned int i = 0; i < str.size(); i++){
		printChar(str[i]);
	}
	printChar('\r');
	printChar('\n');
}

char SerialPort::readChar() {
	return circBuffer.tryDequeue();
}

std::string SerialPort::readLine(){
	std::string str = "";
	char c = '\0';

	while(c != '\n' || c!= '\0') {
		c = circBuffer.tryDequeue();
		str+=c;
	} //what if de-q is faster than data rx

	return str;
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void SerialPort::USCI0RX_ISR(void) {

	if(thisInstance != NULL) {
 	   thisInstance->circBuffer.enqueue(UCA0RXBUF);
 	}
}

