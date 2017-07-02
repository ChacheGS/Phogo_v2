//--------------------------
// **** begin ULTRASOUND ***
//--------------------------
// Based on code by Robi.Wang
#define ULTRASOUND_TRIGGER_PIN 14 // D5
#define ULTRASOUND_ECHO_PIN 12 // D6

void init_ultrasound() {
    pinMode(ULTRASOUND_TRIGGER_PIN, OUTPUT);
    pinMode(ULTRASOUND_ECHO_PIN, INPUT);
    digitalWrite(ULTRASOUND_TRIGGER_PIN, LOW);
}

float measure_distance_cm() {
    // Set to LOW to make a clean pulse
    digitalWrite(ULTRASOUND_TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    // TRIGGER
    digitalWrite(ULTRASOUND_TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    // setup of the ECHO pin before finishing the TRIGGER
    // Measures the duration of the input pulse (it is proportional to the time it takes for the sound to bounce back)
    long microseconds = pulseIn(ULTRASOUND_ECHO_PIN, HIGH,  0.1 * 1000000); // timeout: default (1s)
    digitalWrite(ULTRASOUND_TRIGGER_PIN, LOW);
    return (microseconds / 2.) / 29.1;
}

int measure_distance_cm_filtered(int nsamples) {
    float res = 0;
    int n = nsamples;
    while (n > 0) {
        float dist = measure_distance_cm();
        if (dist > 0) {
            res += dist;
        } else {
            nsamples--;
        }
        n--;
        delay(10);
    }
    //Serial.println(nsamples);
    if (nsamples < 1) {
        return 0;
    }

    return res / nsamples;
}

void setup() {
    // put your setup code here, to run once:
    init_ultrasound();
    Serial.begin(115200);
    delay(200);
}

void loop() {
    // put your main code here, to run repeatedly:
    int dist = measure_distance_cm_filtered(5);
    // float dist = measure_distance_cm(); 
    Serial.println(dist);
    delay(200);
}
