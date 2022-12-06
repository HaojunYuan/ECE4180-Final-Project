#define ALWAYS_CALIBRATING CALIBRATION_LOOPS == -1
#define echoPin 9
#define trigPin 10
long duration; 
int distance; 
int buzzer = 3;

ICommunication* comm;
int loops = 0;
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);  
  Serial.begin(115200);                  
  comm = new SerialCommunication();
  comm->start();
  setupInputs();
}

void loop() {
  if (comm->isOpen()){
    digitalWrite(trigPin, LOW);
    digitalWrite(trigPin, HIGH);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    //If distance <=5cm, alarm the buzzer
    if (distance<=5){
      tone(buzzer, 500, 50);
    }
    #if USING_CALIB_PIN
    bool calibButton = getButton(PIN_CALIB) != INVERT_CALIB;
    if (calibButton)
      loops = 0;
    #else
    bool calibButton = false;
    #endif
    
    bool calibrate = false;
    if (loops < CALIBRATION_LOOPS || ALWAYS_CALIBRATING){
      calibrate = true;
      loops++;
    }
    
    int* fingerPos = getFingerPositions(calibrate, calibButton);
    bool joyButton = getButton(PIN_JOY_BTN) != INVERT_JOY;

    bool menuButton = getButton(PIN_MENU_BTN) != INVERT_MENU;
    
    comm->output(encode(fingerPos, getJoyX(), getJoyY(), joyButton, triggerButton, aButton, bButton, grabButton, pinchButton, calibButton, menuButton));

    #if USING_FORCE_FEEDBACK
      char received[100];
      if (comm->readData(received)){
        int hapticLimits[5];
        if(String(received).length() >= 10) {
           decodeData(received, hapticLimits);
           writeServoHaptics(hapticLimits); 
        }
      }
    #endif
    
    delay(LOOP_TIME);
  }
}
