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

void loop(){
  int pin_status = 0;

  pin_status = digitalRead(REED_SWITCH_INPUT_PIN);
  Serial.print(" ");
  Serial.print(space_status_b4);
  Serial.print(" ");
  Serial.println(space_status);
  delay(DELAY_TIME);

  // pin check of the reed sensor and low pass filter
  if (pin_status == 0){
    if (space_status > 0){
      space_status -= 1;
    }
  } else if (pin_status == 1){
    if (space_status < THRESHOLD){
      space_status += 1;
    }
  }

  // status check if we can switch the status.
  // low pass prevents waggling a bit
  if (space_status >= THRESHOLD-3) {
    // closed
    space_status_b4 = 1;
  } else if (space_status <= 3) {
    // open
    space_status_b4 = 0;
  }

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
