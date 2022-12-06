#if ENCODING == ENCODE_ALPHA
//alphabetic encoding
char* encode(int* jointAngles, int joyX, int joyY, bool joyClick, bool triggerButton,bool calib, bool menu)
{
  static char encodedString[75];

  int trigger = (jointAngles[1] > ANALOG_MAX / 2) ? (jointAngles[1] - ANALOG_MAX / 2) * 2 : 0;

  sprintf(encodedString, "A%dB%dC%dD%dE%dF%dG%dP%d%s%s%s%s\n",
          jointAngles[0], jointAngles[1], jointAngles[2], jointAngles[3], jointAngles[4],
          joyX, joyY, trigger, joyClick ? "H" : "",
          triggerButton ? "I" : "", menu ? "N" : "", calib ? "O" : "");

  return encodedString;
}

//decoding
void decodeData(char* stringToDecode, int* hapticLimits)
{
  hapticLimits[0] = getArgument(stringToDecode, 'A'); //thumb
  hapticLimits[1] = getArgument(stringToDecode, 'B'); //index
  hapticLimits[2] = getArgument(stringToDecode, 'C'); //middle
  hapticLimits[3] = getArgument(stringToDecode, 'D'); //ring
  hapticLimits[4] = getArgument(stringToDecode, 'E'); //pinky
}

#endif