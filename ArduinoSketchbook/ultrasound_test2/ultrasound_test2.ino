// ---------------------------------------------------------------------------
// NewPing library sketch that interfaces with all but the SRF06 sensor using
// only one Arduino pin. You can also interface with the SRF06 using one pin
// if you install a 0.1uf capacitor on the trigger and echo pins of the sensor
// then tie the trigger pin to the Arduino pin (doesn't work with Teensy).
// ---------------------------------------------------------------------------

#include <NewPing.h>

#define PING_PIN  12  // Arduino pin tied to both trigger and echo pins on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

#define PING_TRIGGER 14
#define PING_ECHO 12

NewPing sonar(PING_TRIGGER, PING_ECHO, MAX_DISTANCE); // NewPing setup of pin and maximum distance.
// NewPing sonar(PING_PIN, PING_PIN, MAX_DISTANCE); // NewPing setup of pin and maximum distance.

int measure_distance_cm_filtered(int nsamples) {
    float res = 0;
    int n = nsamples;
    while (n > 0) {
        int dist = sonar.ping_cm();
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
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
}
void loop() {
  delay(200);                     // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  Serial.print("Ping: ");
  Serial.print(sonar.convert_cm(sonar.ping_median(5))); // Send ping, get distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");
}
