int pin = 13;
volatile int state = LOW;

void setup() {
    pinMode(pin, OUTPUT);
    pinMode(2, INPUT);
    attachInterrupt(0, blink, CHANGE);
}

void loop() {
    //delayMicroseconds(50);
    //digitalWrite(pin, state);
    //digitalWrite(pin, state);
     digitalWrite(pin, state);
     
}

void blink() {
    state = !state;
   
}
