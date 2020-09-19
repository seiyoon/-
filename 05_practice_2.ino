#define PIN_LED 7
unsigned int count, toggle;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(115200); // Initialize serial port
  while (!Serial) {
    ; // wait for serial port to connect.
 }
}

void loop() {
    digitalWrite(PIN_LED, 0); //0이 켜짐
    delay(1000);
    int i;
    for(i=0; i<5; i++){
      digitalWrite(PIN_LED, 1);
      delay(100);
      digitalWrite(PIN_LED, 0);
      delay(100);
    }
    digitalWrite(PIN_LED, 1);
    while(1){}
  }
