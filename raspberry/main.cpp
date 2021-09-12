#include <stdio.h>
#include <stdint.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <iostream>

//multi threading
#include <mutex>
#include <chrono>
#include <thread>

#include <bitset>

//mutex
std::mutex mtx_;

//todo:reinterpret_cast someday
namespace andromeda{
	volatile void sread(int connection) {
		//std::lock_guard<std::mutex> lock(mtx_);
		mtx_.lock();
		std::cout << "sread init \n";
		//std::lock_guard <std::mutex> unlock(mtx_);
		mtx_.unlock();
		while (true) {
			mtx_.lock();
			while (serialDataAvail(connection)) {
				//auto data = serialGetchar(connection);
				unsigned char data = serialGetchar(connection);
				if (data != -1) {
					std::cout << "received data : " << std::hex << +data << "\n";

					//std::cout << "received data bin :" << std::bitset<8>(data) << "\n";
				}
				else {
					std::cout << "\n";
				}
			}
			mtx_.unlock();
		}
	}

	volatile void swrite(int connection) {
		mtx_.lock();
		std::cout << "swrite init \n";
		volatile uint8_t counter = 0x00;
		mtx_.unlock();
		while (true) {
			serialPutchar(connection, counter);
			std::cout << "counter:" << std::hex << +counter << std::endl;
			counter++;
			if (counter == 0x20) {
				counter = 0x00;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
	}

	volatile uint32_t readRegister(int connection, uint8_t reg_address = 0x00) {
		int itr = 0;
		uint8_t data[4];
		uint32_t ret = 0;

		serialPutchar(connection, reg_address);

		while (true) {
			while (serialDataAvail(connection)) {
				data[itr] = serialGetchar(connection);
				//std::cout << +data[itr] << std::endl;
				itr++;
			}
			if (itr == 4)
				break;
		}
		ret = data[3] << 24 | data[2] << 16 | data[1] << 8 | data[0];
		return ret;
	}

	void init(int connection) {
		serialPutchar(connection, 0x01);
		serialPutchar(connection, 0x00);
		std::this_thread::sleep_for(std::chrono::microseconds(100));
	}
}

int main(void)
{
	//preparations 
	auto connection = serialOpen("/dev/ttyS0", 115200);
	wiringPiSetup();

	if (connection < 0) {
		std::cout << "couldnt open serial \n";
	}
	andromeda::init(connection);
	
#if 0
	std::thread th_tx(swrite, connection);
	std::thread th_rx(sread, connection);
	
	th_tx.join();
	th_rx.join();
#endif

	uint32_t regdata[32];

	for (int i = 0; i < 32; i++) {
		regdata[i] = andromeda::readRegister(connection, i);
		std::cout << "regnumber:" << i << "   data:" << std::hex << +regdata[i] << std::endl;
		std::this_thread::sleep_for(std::chrono::microseconds(100));
	}


	return 0;
}