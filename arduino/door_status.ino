// door status on arduino
// arduino is Duemilanove
// sensor is a reed sensor

const int REED_SWITCH_INPUT_PIN = 13;

const int RED_LED_OUTPUT_PIN = 12;
const int YELLOW_LED_OUTPUT_PIN = 11;
const int GREEN_LED_OUTPUT_PIN = 10;

void setup(){
 Serial.begin(9600);

 pinMode(REED_SWITCH_INPUT_PIN, INPUT);

 pinMode(RED_LED_OUTPUT_PIN, OUTPUT);
 pinMode(YELLOW_LED_OUTPUT_PIN, OUTPUT);
 pinMode(GREEN_LED_OUTPUT_PIN, OUTPUT);
}

const int DELAY_TIME = 1000;
const int THRESHOLD = 20;

int space_status = THRESHOLD / 2;
int space_status_b4 = 0;

void print_status() {
  Serial.print(" ");
  Serial.print(space_status_b4);
  Serial.print(" ");
  Serial.println(space_status);
}

void update_space_status() {
  if(LOW == digitalRead(REED_SWITCH_INPUT_PIN)) {
    space_status = max(0, space_status - 1);
  } else {
    space_status = min(THRESHOLD, space_status + 1);
  }
}

void loop(){
  print_status();

  update_space_status();

  // status check if we can switch the status.
  // low pass prevents waggling a bit
  if (space_status >= THRESHOLD-3) {
    // closed
    space_status_b4 = 1;
  } else if (space_status <= 3) {
    // open
    space_status_b4 = 0;
  }

  delay(DELAY_TIME);

  // ampel / traffic light signals
  if (space_status_b4 == 1) {
    // closed
    digitalWrite(RED_LED_OUTPUT_PIN, HIGH);
    digitalWrite(GREEN_LED_OUTPUT_PIN, LOW);
  } else if (space_status_b4 == 0) {
    // open
    digitalWrite(RED_LED_OUTPUT_PIN, LOW);
    digitalWrite(GREEN_LED_OUTPUT_PIN, HIGH);
  }

  if (space_status > 3 && space_status < THRESHOLD - 3) {
    digitalWrite(YELLOW_LED_OUTPUT_PIN, HIGH);
  } else {
    digitalWrite(YELLOW_LED_OUTPUT_PIN, LOW);
  }
}
