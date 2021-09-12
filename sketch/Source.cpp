#include <iostream>
#include <bitset>

int main() {
	/*
	uint8_t c[4] = { 0xec,0xf1,0xff,0x7f };
	uint32_t regdata = 0;

	regdata = c[3] << 24 | c[2] << 16 | c[1] << 8 | c[0];

	//std::cout << std::bitset<8>(c[3]) << std::endl;
	//std::cout << std::bitset<32>(regdata) << std::endl;
	std::cout << std::hex << regdata << std::endl;

	*/


	uint8_t d8 = 0xff;

	uint32_t *d32 = reinterpret_cast<uint32_t*>(d8);
	
	std::cout << std::hex << d32;


	return 0;
}