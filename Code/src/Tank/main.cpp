#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <pwm.h>

uint8_t controller_address[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

#define MOTOR_A_POS 12
#define MOTOR_A_NEG 0

#define MOTOR_B_POS 4
#define MOTOR_B_NEG 14

// When an espnow message is recieved, the timeout is set to 15
// When > 0, the espnow_led is on
// When = 0, the espnow_led is off
// Decrements every loop
uint8_t espnow_led_timeout = 0;
#define ESPNOW_LED_TIMEOUT_TIME 30

// speed: [-127, 127] (Fastest going backwards, Fastest going forwards)
void SetLeftMotorSpeed(int8_t speed) {
	
	// Ranges between [0, 255]
	uint8_t absolute_speed = abs(speed) << 1;
	
	if (speed == 0) {
		digitalWrite(MOTOR_A_POS, HIGH);
		digitalWrite(MOTOR_A_NEG, HIGH);
	}
	
	if (speed < 0) {
		digitalWrite(MOTOR_A_NEG, HIGH);
		digitalWrite(MOTOR_A_POS, LOW);
		//analogWrite(MOTOR_A_POS, 255 - absolute_speed);
		return;
	}
	if (speed > 0) {
		digitalWrite(MOTOR_A_POS, HIGH);
		digitalWrite(MOTOR_A_NEG, LOW);
		//analogWrite(MOTOR_A_NEG, 255 - absolute_speed);
		return;
	}
	
	
	
}
void SetRightMotorSpeed(int8_t speed) {
	
	// Ranges between [0, 255]
	uint8_t absolute_speed = abs(speed) << 1;
	
	if (speed == 0) {
		digitalWrite(MOTOR_B_POS, HIGH);
		digitalWrite(MOTOR_B_NEG, HIGH);
	}
	
	if (speed < 0) {
		digitalWrite(MOTOR_B_NEG, HIGH);
		digitalWrite(MOTOR_B_POS, LOW);
		//analogWrite(MOTOR_B_POS, 255 - absolute_speed);
		return;
	}
	if (speed > 0) {
		digitalWrite(MOTOR_B_POS, HIGH);
		digitalWrite(MOTOR_B_NEG, LOW);
		//analogWrite(MOTOR_B_NEG, 255 - absolute_speed);
		return;
	}
	

}

void HandleMessage(uint8_t* message, uint8_t length) {
	
	if (length == 0) { return; }
	
	// Ping command
	if (message[0] == 0x00) {
		if (length != 2) { return; }
		
		if (message[1] == 0x00) {
			Serial.println("Ping recieved");
			uint8_t message[] = {0x00, 0x01};
			//SendMessage(message, 2);
		}
		
	}

	// Left motor speed
	if (message[0] == 0x01) {
		if (length != 2) { return; }
		
		int8_t motorSpeed = (message[1] % 0x40) << 1;
		uint8_t direction = (message[1] >> 6) & 0b1;
		if (direction == 1) {
			motorSpeed *= -1;
		}
		
		Serial.print("Motor_L: ");
		if (direction == 1) { Serial.print("-"); }
		else { Serial.print("+"); }
		Serial.println(motorSpeed);
		
		SetLeftMotorSpeed(motorSpeed);

	}
	
	// Right motor speed
	if (message[0] == 0x02) {
		if (length != 2) { return; }
		
		int8_t motorSpeed = (message[1] % 0x40) << 1;
		uint8_t direction = (message[1] >> 6) & 0b1;
		if (direction == 1) {
			motorSpeed *= -1;
		}
		
		Serial.print("Motor_R: ");
		if (direction == 1) { Serial.print("-"); }
		else { Serial.print("+"); }
		Serial.println(motorSpeed);
		
		SetRightMotorSpeed(motorSpeed);

	}
	
	
	
}

uint8_t serialMessage[255];
uint8_t serialMessageLength = 0;
// Reads the serial messages. If a complete message has been recieved it is passed on to HandleMessage.
void ProcessSerialMessages() {
	
	while (Serial.available() > 0) {
		uint8_t currentByte = Serial.read();
		
		// Start byte
		if (currentByte == 0xFF) {
			serialMessageLength = 0;
			continue;
		}
		
		// End byte
		if (currentByte == 0xFE) {
			HandleMessage(serialMessage, serialMessageLength);
			continue;
		}
		
		serialMessage[serialMessageLength] = currentByte;
		serialMessageLength ++;
		
		
	}

}

#define ESPNOW_MESSAGE_QUEUE_LENGTH 8
uint8_t* espnowMessage[ESPNOW_MESSAGE_QUEUE_LENGTH];
uint8_t espnowMessageLength[ESPNOW_MESSAGE_QUEUE_LENGTH];
void ESPNowCallback(uint8_t *mac_addr, uint8_t *data, uint8_t length) {
	
	espnow_led_timeout = ESPNOW_LED_TIMEOUT_TIME;
	
	uint8_t* espnowData = (uint8_t*)malloc(sizeof(uint8_t) * length);
	memcpy(espnowData, data, length);
	
	for (int i = 0; i < ESPNOW_MESSAGE_QUEUE_LENGTH; i ++) {
		if (espnowMessageLength[i] == 0) {
			espnowMessage[i] = espnowData;
			espnowMessageLength[i] = length;
			break;
		}
	}
	
}
void InitESPNow() {
	
	WiFi.disconnect();
	WiFi.mode(WIFI_STA);
	esp_now_init();
	
	// ESPNow has initialised successfully here
	esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
	esp_now_register_recv_cb(ESPNowCallback);
	
	
}

uint8_t pulseCounter = 0;
void Buzz(int duration, int semitone) {
	
	int freq = 440 * pow(1.05946, semitone);
	
	analogWriteFreq(freq);
	SetRightMotorSpeed(20);
	SetLeftMotorSpeed(-20);
	delay(duration);
	SetRightMotorSpeed(0);
	SetLeftMotorSpeed(0);
	//delay(duration >> 1);
}



void setup() {
	
	Serial.begin(9600);
	Serial.println();
	Serial.println();
	Serial.println("This is the Tank board");
	Serial.print("MAC Address: ");
	Serial.println(WiFi.macAddress());
	
	InitESPNow();

	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(LED_BUILTIN_AUX, OUTPUT);
	
	pinMode(MOTOR_A_POS, OUTPUT);
	pinMode(MOTOR_A_NEG, OUTPUT);
	
	pinMode(MOTOR_B_POS, OUTPUT);
	pinMode(MOTOR_B_NEG, OUTPUT);
	
	
	int duration = 30;
	int tune[] = {-5, 0, 4, 7, 12, -50, -5, 0, 4, 7, 12, -50, -5, 0, 4, 7, 12};
	int length = 17;

	
	//int duration = 120;
	//int tune[] = {2, 2, 14, -50, 9, -50, -50, 8, -50, 7, -50, 5, -50, 2, 5, 7};
	//int length = 16;
	
	
	for (int i = 0; i < length; i ++) {
		if (tune[i] == -50) {
			delay(duration);
			continue;
		}
		Buzz(duration, tune[i]);
	}
	
	
	analogWriteFreq(25000);
	
	SetLeftMotorSpeed(0);
	SetRightMotorSpeed(0);
	
	
}
void loop() {
	
	digitalWrite(MOTOR_A_POS, HIGH);
	digitalWrite(MOTOR_A_NEG, HIGH);
	digitalWrite(MOTOR_B_POS, HIGH);
	digitalWrite(MOTOR_B_NEG, HIGH);
	
	// Turn the builtin led on and off
	if (espnow_led_timeout > 0) {
		digitalWrite(LED_BUILTIN, HIGH);
		espnow_led_timeout -= 1;
	}
	else {
		digitalWrite(LED_BUILTIN, LOW);
	}
	
	// LED Heartbeat
	if (pulseCounter == 0 || pulseCounter == 40) {
		digitalWrite(LED_BUILTIN_AUX, LOW);
		delay(5);
		digitalWrite(LED_BUILTIN_AUX, HIGH);
		delay(5);
	}
	pulseCounter ++;
	pulseCounter %= 160;
	
	// Read and recieve serial messages
	ProcessSerialMessages();
	
	// Check if ESPNow messages are ready to be handled
	for (int i = 0; i < ESPNOW_MESSAGE_QUEUE_LENGTH; i ++) {
		if (espnowMessageLength[i] > 0) {
			HandleMessage(espnowMessage[i], espnowMessageLength[i]);
			free(espnowMessage[i]);
			espnowMessageLength[i] = 0;
		}
	}
	
	delay(5);
	
}