/*
 * Mrf24j.cpp
 *
 *  Created on: Jul 6, 2017
 *      Author: johan
 */

#include "Mrf24j.h"

#include <iostream>

Mrf24j::Mrf24j(
		SPI_HandleTypeDef* spi_handler,
		std::unique_ptr<pinIO> pinReset,
		std::unique_ptr<pinIO> pinCs,
		std::unique_ptr<pinIO> pinInterrupt):
		mSpi_handler(spi_handler),
		pinReset(std::move(pinReset)),
		pinCs(std::move(pinCs)),
		pinInterrupt(std::move(pinInterrupt))
{

}
Mrf24j::Mrf24j(
		SPI_HandleTypeDef* spi_handler,
		std::unique_ptr<pinIO> pinReset):
		mSpi_handler(spi_handler),
		pinReset(std::move(pinReset))
{

}
Mrf24j::~Mrf24j() {
	// TODO Auto-generated destructor stub
}

void Mrf24j::int_callback(){
	mCallback = true;
}

void Mrf24j::reset(void){
	//https://github.com/karlp/Mrf24j40-arduino-library/blob/master/mrf24j.cpp#L1

}


void Mrf24j::run(){

}


void Mrf24j::sendData(uint8_t address, uint8_t value){
	//Mask data correctly before sending.
	address = address << 1;
	address &= 0b01111111;
	address |= 0x01;

	//Enable CS Pin, make MRF aware of incoming message.
	HAL_GPIO_WritePin(MRF_CS_GPIO_Port, MRF_CS_Pin, GPIO_PIN_RESET);

	//Send address and data
	HAL_SPI_Transmit(mSpi_handler,&address, 1, 1);
	HAL_SPI_Transmit(mSpi_handler,&value, 1, 1);

	//Disable transfer to mrf.
	HAL_GPIO_WritePin(MRF_CS_GPIO_Port, MRF_CS_Pin, GPIO_PIN_SET);

	//Delay 1 ms
	HAL_Delay(1);

}
void  Mrf24j::recvData(uint8_t address, uint8_t* result){
	//Mask data correctly before sending.
	address = address << 1;
	address &= 0b01111110;

	//Enable CS Pin, make MRF aware of incoming message.
	HAL_GPIO_WritePin(MRF_CS_GPIO_Port, MRF_CS_Pin, GPIO_PIN_RESET);

	//Send address and receive data
	HAL_SPI_Transmit(mSpi_handler,&address, bufferSize, 1);
	HAL_SPI_Receive(mSpi_handler,result,bufferSize, 1);

	//Disable transfer to mrf.
	HAL_GPIO_WritePin(MRF_CS_GPIO_Port, MRF_CS_Pin, GPIO_PIN_SET);

	//Delay 1 ms
	HAL_Delay(1);
}

void Mrf24j::sendLongData(uint16_t address, uint8_t value)
{
	//Mask data correctly before sending.
	uint8_t address1 = (((uint8_t)(address>>3))&0x7F)|0x80;
	uint8_t address2 = (((uint8_t)(address<<5))&0xE0)|0x10;

	//Enable CS Pin, make MRF aware of incoming message.
	HAL_GPIO_WritePin(MRF_CS_GPIO_Port, MRF_CS_Pin, GPIO_PIN_RESET);

	//Send address and data
	HAL_SPI_Transmit(mSpi_handler, &address1, 1, 1);
	HAL_SPI_Transmit(mSpi_handler, &address2, 1, 1);
	HAL_SPI_Transmit(mSpi_handler, &value, 1, 1);

	//Disable transfer to mrf.
	HAL_GPIO_WritePin(MRF_CS_GPIO_Port, MRF_CS_Pin, GPIO_PIN_SET);

	//Delay 1 ms
	HAL_Delay(1);
}
void Mrf24j::readLongData(uint16_t address, uint8_t* result){
	//Mask data correctly before reading.
	uint8_t address1 = ((address>>3)&0x7F)|0x80;
	uint8_t address2 = ((address<<5)&0xE0);

	//Enable CS Pin, make MRF aware of incoming message.
	HAL_GPIO_WritePin(MRF_CS_GPIO_Port, MRF_CS_Pin, GPIO_PIN_RESET);

	//Send address and receieve data
	HAL_SPI_Transmit(mSpi_handler,&address1, 1, 1);
	HAL_SPI_Transmit(mSpi_handler, &address2, 1, 1);
	HAL_SPI_Receive(mSpi_handler, result, 1, 1);

	//Disable transfer to mrf.
	HAL_GPIO_WritePin(MRF_CS_GPIO_Port, MRF_CS_Pin, GPIO_PIN_SET);

	//Delay 1 ms
	HAL_Delay(1);
}