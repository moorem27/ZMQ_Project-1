int incoming_byte = 0;

void setup() {
    Serial.begin(9600);
    pinMode(13, OUTPUT);
}


void loop() {
    if(Serial.available() > 0) {
        incoming_byte = Serial.read();
        blink_LED();
    }
}


void blink_LED() {
    digitalWrite(13, HIGH);
    delay(50);
    digitalWrite(13, LOW);
    delay(50);
}
