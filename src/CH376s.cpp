#include <Arduino.h>
#include "CH376s.h"

CH376s::CH376s(uint8_t*  pins, uint8_t RD, uint8_t WR, uint8_t CS, uint8_t A){
	for(int i=0; i<8; i++){
	    this->_portPins[i] = pins[i];
	}
	this->_RD = RD;
	this->_WR = WR;
	this->_CS = CS;
	this->_A = A;

	pinMode(this->_WR,OUTPUT);
	pinMode(this->_RD,OUTPUT);
	pinMode(this->_A,OUTPUT);
}

/*
*Asigna el valor determinado en el puerto
*@param value el valor a escribir en el puerto
*/
void CH376s::portWrite(uint8_t value){
	for(uint8_t i=0; i<8; i++){
	    pinMode(this->_portPins[i], OUTPUT);
	}

	for(uint8_t i=0; i<8; i++){
	    if((value & 0x80)==0x80)digitalWrite(this->_portPins[i], HIGH);
	    else if((value & 0x80)==0)digitalWrite(this->_portPins[i], LOW);
	    value = value << 1;
	}
}

/*
*Devuelve el valor que hay en el puerto
*Es importante tener todos los pines en nivel alto o bajo
*si no esta conectado alguno puede producir mediciones 
*incorrectas
*@return el valor leido
*/
uint8_t CH376s::portRead(){
	uint8_t result=0;
	uint8_t aux=0x80;

	for(uint8_t i=0; i<8; i++){
	    pinMode(this->_portPins[i], INPUT);
	}

	for(uint8_t i=0; i<8; i++){
	    result += digitalRead(_portPins[i])* aux;
	    aux = aux >> 1;
	}
	return result;
}


void CH376s::CMD(uint8_t cmd){

  	digitalWrite(this->_WR, HIGH);
  	digitalWrite(this->_RD, HIGH);
  	digitalWrite(this->_A,LOW);

  	this->portWrite(0);

  	digitalWrite(this->_A, HIGH);
  	//delayMicroseconds(1);

  	this->portWrite(cmd);

  	digitalWrite(this->_WR, LOW);
  	//delayMicroseconds(1);
  	digitalWrite(this->_WR, HIGH);
}

uint8_t CH376s::read(){
	digitalWrite(this->_A, LOW);
	digitalWrite(this->_RD, LOW);
	//delayMicroseconds(1);
  	byte a = this->portRead();
  	digitalWrite(this->_RD, HIGH);
  	return a;
}

void CH376s::write(uint8_t data){
	digitalWrite(this->_A, LOW);
  	//delayMicroseconds(1);
  	this->portWrite((byte)data);
  	//delayMicroseconds(10);
  	digitalWrite(this->_WR, LOW);
  	//delayMicroseconds(1);
  	digitalWrite(this->_WR, HIGH);
}

bool CH376s::checkExist(){
	this->CMD((byte)0x06);
	this->write((byte)0x06);
	byte result = this->read();
	if(result = 249) return true;
	else return false;
}

bool CH376s::setUSBMode(uint8_t data){
	this->CMD((byte)0x15);
	this->write(data);
	byte result = this->read();
	if(result = 0x81) return true;
	else return false;
}

void CH376s::resetAll(){
	this->CMD(0x05);
	delay(200);
}

void CH376s::setFileName(String nombre){
	this->CMD(0x2F);
	this->write(0x2F);
	byte size = sizeof(nombre);
	byte bytes[size];
	nombre.getBytes(bytes, size);
	for(int i=0; i<size; i++){
	    this->write(bytes[i]);
	}
	this->write((byte)0x00);
	delay(20);
}

bool CH376s::diskConnect(){
	pinMode(30, INPUT_PULLUP);
	this->CMD((byte)0x30);
	if(digitalRead(30)==LOW)Serial.println("SI");
	byte result = this->read();
	Serial.println(result, HEX);
}

bool CH376s::diskMount(){
	this->CMD(0x31);
	byte result = this->read();
	while(digitalRead(30)==LOW)Serial.println("SI");
	Serial.println(result,HEX);
	if(result==0x14)return true;
	else return false;
}

bool CH376s::fileCreate(){
	this->CMD(0x34);
	byte result = this->read();
	if(result==0x14)return true;
}

bool CH376s::fileClose(){
	this->CMD(0x36);
	this->write(0x01);
}

uint8_t CH376s::getStatus(){
	this->CMD(0x22);
	return this->read();
}