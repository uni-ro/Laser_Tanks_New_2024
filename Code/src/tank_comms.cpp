#include "tank_comms.h"

#define TANK_ADDRESS {0x80, 0x7D, 0x3A, 0x23, 0x7F, 0x58}

uint8_t* target_address;

void TankCommsInit(uint8_t* tank_address) {
	
	tank_address = (uint8_t*)malloc(6 * sizeof(uint8_t));
	memcpy(target_address, tank_address, 6);
	
	
	WiFi.disconnect();
	WiFi.mode(WIFI_STA);
	
	esp_now_init();
	
	esp_now_peer_info_t info;
	info.channel = 0;
	info.encrypt = false;
	memcpy(info.peer_addr, target_address, 6);
	esp_now_add_peer(&info);
	
}

// left_speed: [-127, 127] (Fastest going backwards, Fastest going forwards)
// right_speed: [-127, 127] (Fastest going backwards, Fastest going forwards)
void SetMotorSpeed(int8_t left_speed, int8_t right_speed) {
	
	uint8_t absolute_left_speed = left_speed & 0b01111111;
	uint8_t absolute_right_speed = right_speed & 0b01111111;
	
	uint8_t left_direction = (left_speed & 0b10000000) >> 7;
	uint8_t right_direction = (right_speed & 0b10000000) >> 7;
	
	
	
	uint8_t left_speed_argument = (absolute_left_speed >> 1) + 0x40 * left_direction;
	uint8_t right_speed_argument = (absolute_left_speed >> 1) + 0x40 * left_direction;
	
	
	uint8_t left_motor_command[] = {0x01, left_speed_argument};
	uint8_t right_motor_command[] = {0x02, right_speed_argument};

	esp_now_send(target_address, left_motor_command, 2);
	esp_now_send(target_address, right_motor_command, 2);

}