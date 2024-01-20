#include <Arduino.h>
#include <PCA9685.h>

#define F_LEFT  1
#define F_RIGHT 2
#define B_LEFT  3
#define B_RIGHT 4
#define PWM_MIN  150
#define PWM_MAX  600

struct leg {
    int coxa;
    int femur;
    int tibia;
};

struct leg legs[4];
PCA9685 pwm(0x40);

void servoWrite(uint8_t channel, int deg) {
    uint16_t pulselen = map(deg, 0, 180, PWM_MIN, PWM_MAX);
    pwm.setPWM(channel, pulselen);
}

void setup() {
    Serial.begin(9600);
    Wire.begin();
    pwm.begin();
    pwm.setFrequency(50);
    
    for (int i = 0; i < 4; i++) {
        legs[i].tibia = 3*i;
        legs[i].femur = 3*i + 1;
        legs[i].coxa  = 3*i + 2;

    }
    
    servoWrite(legs[0].tibia, 90);
    servoWrite(legs[0].femur, 90);
    servoWrite(legs[0].coxa,  90);
}

void loop() {

}
