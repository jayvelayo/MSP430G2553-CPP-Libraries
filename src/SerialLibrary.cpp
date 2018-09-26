/**
 * SerialLibrary.cpp
 *
 */

#include "SerialLibrary.hpp"

CircularBuffer::CircularBuffer() {
	startIndex = 0;
	stopIndex = 0;
	isFull = FALSE;
}

bool CircularBuffer::enqueue(char newByte) {
	if(isFull) {
		return FALSE;
	}
	else {
		charBuffer[stopIndex++]= newByte;
		if(stopIndex >= MAX_BUFF_SIZE) { stopIndex = 0; }
		if(stopIndex == startIndex) { isFull = TRUE; }
		return TRUE;
	}
}

char CircularBuffer::tryDequeue(void) {
	if(CircularBuffer::getBufferLength() == 0) {
		return '0';
	}
	else {
		char returnChar = charBuffer[startIndex++];
		if(startIndex>=MAX_BUFF_SIZE) {startIndex = 0;}
		if(isFull) { isFull=FALSE; }
		return returnChar;
	}
}

int CircularBuffer::getBufferLength() {
	if(isFull) return MAX_BUFF_SIZE;
	else if (stopIndex >= startIndex) return stopIndex-startIndex;
	else return (MAX_BUFF_SIZE - startIndex - stopIndex);
}

bool CircularBuffer::isEmpty() {
	if (startIndex==stopIndex && !isFull) return TRUE;
	else return FALSE;
}

SerialPort::SerialPort() {
}

void SerialPort::begin() {
	UCA0CTL1 |= UCSSEL_2 + UCSWRST;                     // SMCLK
    UCA0BR0 = 104;                            // 1MHz 9600
    UCA0BR1 = 0;                              // 1MHz 9600
    UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
    P1DIR |= BIT1+BIT2;
    P1SEL |= BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
    P1SEL2 |= BIT1 + BIT2 ;                    // P1.1 = RXD, P1.2=TXD
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
}

void SerialPort::txChar(char c) {
	while (!(IFG2&UCA0TXIFG));
    UCA0TXBUF = c;
}


