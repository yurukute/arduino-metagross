#include <Arduino.h>
#include <PCA9685.h>

#define F_LEFT  1
#define F_RIGHT 2
#define B_LEFT  3
#define B_RIGHT 4
#define PWM_MIN  150
#define PWM_MAX  450

struct leg {
    int coxa;
    int femur;
    int tibia;
};

struct leg legs[4];
PCA9685 pwm(0x40);

int servoWrite(uint8_t channel, int deg) {
    uint16_t pulselen = map(deg, 0, 180, PWM_MIN, PWM_MAX);
    if (pwm.isConnected())
        pwm.setPWM(channel, pulselen);
    else
        Serial.println("PWM not available");
    if (pwm.lastError() == PCA9685_OK) {
        Serial.print("channel " + String(channel) + ": " + deg);
        Serial.println(" (" + String(pulselen) + "WPM)");
    }
    delay(500);
    return pwm.lastError();

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
}

void stop(int deg=90) {
    for (int i = 0; i < 4; i++) {
        servoWrite(legs[i].coxa, deg);
    }
}

void sweep(int from, int to) {
    int direction = 1;
    if (from > to) {
        direction = -1;
    }
    for (int deg = from; deg != to; deg+=10*(direction)) {
        for (int i = 0; i < 4; i++) {
            servoWrite(legs[i].coxa, deg);
        }
    }
}

void loop() {
    stop();
}
