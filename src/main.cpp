#include <Arduino.h>
#include <PCA9685.h>

#define F_LEFT  0
#define F_RIGHT 1
#define B_LEFT  2
#define B_RIGHT 3
#define PWM_MIN  150
#define PWM_MAX  500 // Original 600

struct leg {
    int coxa;
    int femur;
    int tibia;
};

struct leg legs[4];
PCA9685 pwm(0x40);

void setup() {
    Serial.begin(9600);
    Wire.begin();
    pwm.begin();
    pwm.setFrequency(50); // SG-90

    for (int i = 0; i < 4; i++) {
        legs[i].coxa  = 3*i;
        legs[i].femur = 3*i + 1;
        legs[i].tibia = 3*i + 2;
    }
}

int servoWrite(uint8_t channel, int deg) {
    uint16_t pulselen = map(deg, 0, 180, PWM_MIN, PWM_MAX);

    pwm.setPWM(channel, pulselen);
    Serial.print("Write channel " + String(channel) + ": ");

    int err = pwm.lastError();

    if (err == PCA9685_OK)
        Serial.println(String(deg) + " (" + pulselen + "WPM)");
    else
        Serial.println("failed. ERR: " + String(err));

    delay(500);
    return pwm.lastError();
}

// Sync all servo at the given degree (90 by default)
void sync(int deg=90) {
    for (int i = 0; i < 4; i++) {
        servoWrite(legs[i].coxa, deg);
        servoWrite(legs[i].femur, deg);
        servoWrite(legs[i].tibia, deg);
    }
}

void sweep(int servo, int from, int to, int step=10) {
    if (from + step > 180 || step <= 0) {
        Serial.println("Invalid speed");
        return;
    }
    int direction = (from > to ? -1 : 1);
    for (int deg = from; deg != to; deg += step*direction) {
        servoWrite(servo, deg);    
    }
}

void stop() {
    for (int i = 0; i < 4; i++) {
        servoWrite(legs[i].coxa, 90);
        if (i == F_LEFT || i == B_RIGHT) {
            servoWrite(legs[i].femur, 40);
            servoWrite(legs[i].tibia, 180);
        }
        else { // F_RIGHT || B_LEFT
            servoWrite(legs[i].femur, 140);
            servoWrite(legs[i].tibia,0 );
        }
    }
}

void loop() {
    //pwm.allOFF();
    stop();
}
