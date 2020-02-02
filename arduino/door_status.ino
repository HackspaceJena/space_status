// door status on arduino
// arduino is Duemilanove
// sensor is a reed sensor

#define REED_SWITCH_INPUT_PIN 13

#define SPACE_STATUS_RED_PIN 12
#define SPACE_STATUS_YELLOW_PIN 11
#define SPACE_STATUS_GREEN_PIN 10

void setup(){
 Serial.begin(9600);
 pinMode(REED_SWITCH_INPUT_PIN, INPUT);

 pinMode(SPACE_STATUS_RED_PIN, OUTPUT);
 pinMode(SPACE_STATUS_YELLOW_PIN, OUTPUT);
 pinMode(SPACE_STATUS_GREEN_PIN, OUTPUT);
}

int threshold = 20;
int space_status = threshold / 2;
int space_status_b4 = 0;
int delay_time = 1000;

void loop(){
  int pin_status = 0;

  pin_status = digitalRead(REED_SWITCH_INPUT_PIN);
  Serial.print(" ");
  Serial.print(space_status_b4);
  Serial.print(" ");
  Serial.println(space_status);
  delay(delay_time);

  // pin check of the reed sensor and low pass filter
  if (pin_status == 0){
    if (space_status > 0){
      space_status -= 1;
    }
  } else if (pin_status == 1){
    if (space_status < threshold){
      space_status += 1;
    }
  }

  // status check if we can switch the status.
  // low pass prevents waggling a bit
  if (space_status >= threshold-3) {
    // closed
    space_status_b4 = 1;
  } else if (space_status <= 3) {
    // open
    space_status_b4 = 0;
  }

  // ampel / traffic light signals
  if (space_status_b4 == 1) {
    // closed
    digitalWrite(SPACE_STATUS_RED_PIN, HIGH);
    digitalWrite(SPACE_STATUS_GREEN_PIN, LOW);
  } else if (space_status_b4 == 0) {
    // open
    digitalWrite(SPACE_STATUS_RED_PIN, LOW);
    digitalWrite(SPACE_STATUS_GREEN_PIN, HIGH);
  }

  if (space_status > 3 && space_status < threshold - 3) {
    digitalWrite(SPACE_STATUS_YELLOW_PIN, HIGH);
  } else {
    digitalWrite(SPACE_STATUS_YELLOW_PIN, LOW);
  }
}
