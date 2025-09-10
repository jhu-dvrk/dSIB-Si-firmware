#include <Arduino.h>

int led = PB9;
int button = PB8;
int tx = PB6;
int rx = PB7;
int pin_int = PB5;

int tx2 = PB3;
int rx2 = PA15;

int brake1 = PB14_ALT1;
int brake2 = PB15_ALT2;
int brake3 = PA8;
int brake4 = PA9;
int brake5 = PA10;

int sense1 = PA1;
int sense2 = PA5;
int sense3 = PA4;
int sense4 = PA3;
int sense5 = PA2;
int sense_mv = PA0;

uint32_t last_release = 0;
int is_released = 0;

int DEBOUNCE_TIME = 100;
int HIGH_POWER_TIME = 200;
int HIGH_POWER = 40 * 255 / 48;
int LOW_POWER = 35 * 255 / 48;
int HIGH_POWER_Z = 45 * 255 / 48;
int LOW_POWER_Z = 40 * 255 / 48;

HardwareSerial serial_arm(rx2, tx2);


void setup() {
    Serial.begin(9600);
    serial_arm.begin(115200);
    pinMode(led, OUTPUT);
    pinMode(rx, INPUT_PULLUP);
    pinMode(button, INPUT);

    pinMode(brake1, OUTPUT);
    pinMode(brake2, OUTPUT);
    pinMode(brake3, OUTPUT);
    pinMode(brake4, OUTPUT);
    pinMode(brake5, OUTPUT);
    digitalWrite(brake1, 0);
    digitalWrite(brake2, 0);
    digitalWrite(brake3, 0);
    digitalWrite(brake4, 0);
    digitalWrite(brake5, 0);

    analogWriteFrequency(20000);
}

void loop () {
    uint32_t t = millis();
    while (serial_arm.available()) {
      Serial.write(serial_arm.read());
    }
    if (digitalRead(rx) == 0 || digitalRead(button) == 1) {
        if (!is_released) {
            last_release = t;
            is_released = 1;
        }
    } else {
        is_released = 0;
    }
    if (is_released) {
        if (t - last_release < DEBOUNCE_TIME) {
          // do nothing
        } else if (t - last_release < DEBOUNCE_TIME + HIGH_POWER_TIME) {
            analogWrite(brake1, HIGH_POWER_Z);
            analogWrite(brake2, HIGH_POWER);
            analogWrite(brake3, HIGH_POWER);
            analogWrite(brake4, HIGH_POWER);
            analogWrite(brake5, HIGH_POWER);  
            digitalWrite(led, 1);

        } else {
            analogWrite(brake1, LOW_POWER_Z);
            analogWrite(brake2, LOW_POWER);
            analogWrite(brake3, LOW_POWER);
            analogWrite(brake4, LOW_POWER);
            analogWrite(brake5, LOW_POWER);     
            digitalWrite(led, (millis() % 100) > 50 ? 1 : 0);

        }
    } else {
        analogWrite(brake1, 0);
        analogWrite(brake2, 0);
        analogWrite(brake3, 0);
        analogWrite(brake4, 0);
        analogWrite(brake5, 0);
        digitalWrite(led, 1);

    }
}
