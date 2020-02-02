/* Door state monitoring logic for Arduino Duemilanove
 *
 * This script periodically reads the state of a reed switch
 * (REED_SWITCH_INPUT_PIN) to determine the locked state of door.
 *
 * The switch sometimes reports the wrong state for brief periods of time.
 * This script filters out these quick changes. A counter
 * (measured_state_counter) is decremented or incremented within a range
 * from 0 to MAX_COUNTER depending on the state of the reed switch in each
 * iteration of the loop. The reported state (reported_state via
 * RED_LED_OUTPUT_PIN and GREEN_LED_OUTPUT_PIN) is only changed, if the counter
 * reaches its lower (0 to LOWER_THRESHOLD) or upper end (UPPER_THRESHOLD to
 * MAX_COUNTER). The scripts also reports when the state counter is between the
 * lower and upper end (YELLOW_LED_OUTPUT_PIN).
 *
 * State counter and reported state are written to the serial port in each
 * iteration for debugging purposes.
 */

const int REED_SWITCH_INPUT_PIN = 13;

const int RED_LED_OUTPUT_PIN = 12;
const int YELLOW_LED_OUTPUT_PIN = 11;
const int GREEN_LED_OUTPUT_PIN = 10;

const int DELAY_TIME = 1000;

const int MAX_COUNTER = 20;
const int LOWER_THRESHOLD = 4;
const int UPPER_THRESHOLD = MAX_COUNTER - LOWER_THRESHOLD;

const int CLOSED_DOOR = 1;
const int OPEN_DOOR = 0;

int measured_state_counter = MAX_COUNTER / 2;
int reported_state = OPEN_DOOR;

void setup(){
 Serial.begin(9600);

 pinMode(REED_SWITCH_INPUT_PIN, INPUT);

 pinMode(RED_LED_OUTPUT_PIN, OUTPUT);
 pinMode(YELLOW_LED_OUTPUT_PIN, OUTPUT);
 pinMode(GREEN_LED_OUTPUT_PIN, OUTPUT);
}

void loop(){
  // print state
  Serial.print(" ");
  Serial.print(reported_state);
  Serial.print(" ");
  Serial.println(measured_state_counter);

  // update measured state
  if(LOW == digitalRead(REED_SWITCH_INPUT_PIN)) {
    measured_state_counter = max(0, measured_state_counter - 1);
  } else {
    measured_state_counter = min(THRESHOLD, measured_state_counter + 1);
  }

  // update reported state
  if (measured_state_counter > UPPER_THRESHOLD) {
    reported_state = CLOSED_DOOR;
  } else if (measured_state_counter < LOWER_THRESHOLD) {
    reported_state = OPEN_DOOR;
  }

  delay(DELAY_TIME);

  // update led pins
  if (reported_state == CLOSED_DOOR) {
    digitalWrite(RED_LED_OUTPUT_PIN, HIGH);
    digitalWrite(GREEN_LED_OUTPUT_PIN, LOW);
  } else if (reported_state == OPEN_DOOR) {
    digitalWrite(RED_LED_OUTPUT_PIN, LOW);
    digitalWrite(GREEN_LED_OUTPUT_PIN, HIGH);
  }
  if (measured_state_counter == constrain(measured_state_counter, LOWER_THRESHOLD, UPPER_THRESHOLD)) {
    digitalWrite(YELLOW_LED_OUTPUT_PIN, HIGH);
  } else {
    digitalWrite(YELLOW_LED_OUTPUT_PIN, LOW);
  }
}
