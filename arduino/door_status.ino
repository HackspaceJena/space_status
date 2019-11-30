# arduino Duemilanove
#define IR_INPUT_PIN A0
#define IR_LED_PIN 13

#define status_red_PIN 12
#define status_yellow_PIN 11
#define status_green_PIN 10



void setup(){
 Serial.begin(9600); 
 pinMode(IR_INPUT_PIN, INPUT);
 pinMode(IR_LED_PIN, OUTPUT);

 pinMode(status_red_PIN, OUTPUT);
 pinMode(status_yellow_PIN, OUTPUT);
 pinMode(status_green_PIN, OUTPUT);
}


int threshold = 900;
int value_b4 = 900;

int value = 0;

void loop(){
  int ambient = 0;
  int lit = 0;
  
  
  digitalWrite(IR_LED_PIN, LOW);
  delay(5);                            //To give ADC and LED transition time
  ambient = analogRead(IR_INPUT_PIN);
  
  digitalWrite(IR_LED_PIN, HIGH);
  delay(5);
  lit = analogRead(IR_INPUT_PIN);
  
  value = ((lit - ambient) + value_b4) / 2; // small filter
  
  Serial.print(value);
  Serial.print(" ");
  Serial.println(ambient);
  delay(500);

  if (value >= threshold) {
    digitalWrite(status_red_PIN, HIGH);
    digitalWrite(status_yellow_PIN, LOW);
    digitalWrite(status_green_PIN, LOW);
  }

  //if (value < 977 && value >= 964) {
  //  digitalWrite(status_red_PIN, LOW);
  //  digitalWrite(status_yellow_PIN, HIGH);
  //  digitalWrite(status_green_PIN, LOW);
  //}

  if (value < threshold) {
    digitalWrite(status_red_PIN, LOW);
    digitalWrite(status_yellow_PIN, LOW);
    digitalWrite(status_green_PIN, HIGH);
  }

  value_b4 = value;
}
