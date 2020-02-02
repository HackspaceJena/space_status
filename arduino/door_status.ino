// door status on arduino
// arduino is Duemilanove
// sensor is a reed sensor

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
  print_state();

  update_measured_state_counter();

  update_reported_state();

  delay(DELAY_TIME);

  update_led_pins();
}

void print_state() {
  Serial.print(" ");
  Serial.print(reported_state);
  Serial.print(" ");
  Serial.println(measured_state_counter);
}

void update_measured_state_counter() {
  if(LOW == digitalRead(REED_SWITCH_INPUT_PIN)) {
    measured_state_counter = max(0, measured_state_counter - 1);
  } else {
    measured_state_counter = min(THRESHOLD, measured_state_counter + 1);
  }
}

  // status check if we can switch the status.
  // low pass prevents waggling a bit
void update_reported_state() {
  if (measured_state_counter > UPPER_THRESHOLD) {
    reported_state = CLOSED_DOOR;
  } else if (measured_state_counter < LOWER_THRESHOLD) {
    reported_state = OPEN_DOOR;
  }
}

void update_led_pins() {
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
