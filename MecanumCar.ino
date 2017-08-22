#define DEBUG

#define FORWARD = 0x1;
#define REVERSE = 0x0;


byte CH1_PIN = A0;
byte CH2_PIN = A1;
byte CH3_PIN = A2;
byte CH4_PIN = A3;


int CH1_DATA ;
int CH2_DATA ;
int CH3_DATA ;
int CH4_DATA ;

byte T_PIN = 3; 

//Motor enable pins (pulse for speed)
byte RightSpeed_PIN = 9; //Right Wheels
byte LeftSpeed_PIN = 10; //Left Wheels

byte RR_DIR[] = { 6,5 }; //Right Rear 
byte RF_DIR[] = { 8,7 }; //Right Front 
byte LF_DIR[] = { 12,11 }; //Left Front
byte LR_DIR[] = { 13,4 }; //Left Rear




void setup() {
  pinMode(CH1_PIN, INPUT);
  pinMode(CH2_PIN, INPUT);
  pinMode(CH3_PIN, INPUT);
  pinMode(CH4_PIN, INPUT);

  pinMode(T_PIN,OUTPUT);

  //Set motor speed pins to output
  pinMode(RightSpeed_PIN,OUTPUT);
  pinMode(LeftSpeed_PIN, OUTPUT);
  
  //Motor Direction
  pinMode(RR_DIR[0], OUTPUT);
  pinMode(RR_DIR[1], OUTPUT);
  pinMode(RF_DIR[0], OUTPUT);
  pinMode(RF_DIR[1], OUTPUT);
  pinMode(LR_DIR[0], OUTPUT);
  pinMode(LR_DIR[1], OUTPUT);
  pinMode(LF_DIR[0], OUTPUT);
  pinMode(LF_DIR[1], OUTPUT);
  
  #if defined(DEBUG)
	Serial.begin(115200);
  Serial.println("Waiting for controller to connect");
  #endif

  //Wait for data channel to NOT timeout
	while(pulseIn(CH1_PIN, HIGH) == 0){
	}


#if defined(DEBUG)
  Serial.println("Controller connected. Waiting to center joysticks");
#endif

  readData();
  while(CH3_DATA != 0x0) {
    readData();
  }
}

void loop() {
  digitalWrite(T_PIN, !digitalRead(T_PIN)); //Toggle pin for loop timming
  
  readData();
  
  if (abs(CH4_DATA) > 14) {
	//rotate wheels in oposite directions 
	setSpeed(RightSpeed_PIN, CH4_DATA);
	setDirection(RR_DIR, CH4_DATA*-1);
	setDirection(RF_DIR, CH4_DATA);
 
  }
  else {
	setSpeed(RightSpeed_PIN, CH2_DATA);
	setDirection(RR_DIR, CH2_DATA);
	setDirection(RF_DIR, CH2_DATA);
 }

  
  if (abs(CH1_DATA) > 14) {
	//rotate wheels in oposite directions 
	setSpeed(LeftSpeed_PIN, CH1_DATA);
	setDirection(LR_DIR, CH1_DATA);
	setDirection(LF_DIR, CH1_DATA*-1);

  }
  else {
	setSpeed(LeftSpeed_PIN, CH3_DATA);
	setDirection(LR_DIR, CH3_DATA);
	setDirection(LF_DIR, CH3_DATA);
	
  }
  



	#if defined(DEBUG)
		Serial.print(" CH1:");
		Serial.print(CH1_DATA);
		Serial.print(" CH2:");
		Serial.print(CH2_DATA);
		Serial.print(" CH3:");
		Serial.print(CH3_DATA);
		Serial.print(" CH4:");
		Serial.print(CH4_DATA);

    Serial.println();
	#endif


	
 }


//speed should be between 0 and 25 (0-255)
void setSpeed(byte motorSpeedPin, int speed) {

	speed = abs(speed);
	if (speed >= 25)
		speed = 255;
	else if (speed > 6) //0-6 is dead zone, too slow for motor to engage
		speed = speed * 10;
  else
   speed = 0;

	analogWrite(motorSpeedPin, speed);
  
	#if defined(DEBUG)
		Serial.print(" speed:");
		Serial.print(speed);
	#endif
}


void setDirection(byte pin[], int channelData) {
	digitalWrite(pin[0], channelData > 0);
	digitalWrite(pin[1], channelData < 0);
  }



 void readData() {
  CH1_DATA = map(pulseIn(CH1_PIN, HIGH),995,1970,-25,25);
  CH2_DATA = map(pulseIn(CH2_PIN, HIGH),995,1970,-25,25);
  CH3_DATA = map(pulseIn(CH3_PIN, HIGH),995,1970,-25,25);
  CH4_DATA = map(pulseIn(CH4_PIN, HIGH),995,1970,-25,25);
}

