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
const int CLOSED_DOOR = 1;
const int OPEN_DOOR = 0;

int space_status = THRESHOLD / 2;
int published_door_state = OPEN_DOOR;

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

void update_space_status_b4() {
  // status check if we can switch the status.
  // low pass prevents waggling a bit
  if (space_status >= THRESHOLD-3) {
    space_status_b4 = CLOSED_DOOR;
  } else if (space_status <= 3) {
    space_status_b4 = OPEN_DOOR;
  }
}

void loop(){
  print_status();

  update_space_status();

  update_space_status_b4();

  delay(DELAY_TIME);

  // ampel / traffic light signals
  if (space_status_b4 == CLOSED_DOOR) {
    digitalWrite(RED_LED_OUTPUT_PIN, HIGH);
    digitalWrite(GREEN_LED_OUTPUT_PIN, LOW);
  } else if (space_status_b4 == OPEN_DOOR) {
    digitalWrite(RED_LED_OUTPUT_PIN, LOW);
    digitalWrite(GREEN_LED_OUTPUT_PIN, HIGH);
  }

  if (space_status > 3 && space_status < THRESHOLD - 3) {
    digitalWrite(YELLOW_LED_OUTPUT_PIN, HIGH);
  } else {
    digitalWrite(YELLOW_LED_OUTPUT_PIN, LOW);
  }
}
