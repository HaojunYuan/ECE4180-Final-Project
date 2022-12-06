int maxFingers[5] = {0,0,0,0,0};
int minFingers[5] = {ANALOG_MAX, ANALOG_MAX, ANALOG_MAX, ANALOG_MAX, ANALOG_MAX};

void setupInputs(){
  pinMode(PIN_JOY_BTN, INPUT_PULLUP);
  pinMode(PIN_A_BTN, INPUT_PULLUP);
  pinMode(PIN_B_BTN, INPUT_PULLUP);
  pinMode(PIN_MENU_BTN, INPUT_PULLUP);
}

int* getFingerPositions(bool calibrating, bool reset){
  int rawFingers[5] = {NO_THUMB?0:analogRead(PIN_THUMB), analogRead(PIN_INDEX), analogRead(PIN_MIDDLE), analogRead(PIN_RING), analogRead(PIN_PINKY)};

  #if FLIP_POTS
  for (int i = 0; i <5; i++){
    rawFingers[i] = ANALOG_MAX - rawFingers[i];
  }
  #endif

  #if ENABLE_MEDIAN_FILTER
  for (int i = 0; i < 5; i++){
    rmSamples[i].add( rawFingers[i] );
    rawFingers[i] = rmSamples[i].getMedian();
  }
  #endif

  //reset max and mins as needed
  if (reset){
    for (int i = 0; i <5; i++){
      maxFingers[i] = 0;
      minFingers[i] = ANALOG_MAX;
    }
  }

  static int calibrated[5] = {511,511,511,511,511};
  
  for (int i = 0; i<5; i++){
    if (minFingers[i] != maxFingers[i]){
      calibrated[i] = map( rawFingers[i], minFingers[i], maxFingers[i], 0, ANALOG_MAX );
      #if CLAMP_ANALOG_MAP
        if (calibrated[i] < 0)
          calibrated[i] = 0;
        if (calibrated[i] > ANALOG_MAX)
          calibrated[i] = ANALOG_MAX;
      #endif
    }
    else {
      calibrated[i] = ANALOG_MAX / 2;
    }
  }
  return calibrated;
  
}

bool getButton(byte pin){
  return digitalRead(pin) != HIGH;
}
