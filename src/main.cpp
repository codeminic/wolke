#include <Arduino.h>

// ESP32S3 Pins
#define D0 1
#define D1 2
#define D2 3
#define D3 4
#define D4 5
#define D5 6
#define D6 43
#define D7 44
#define D8 7
#define D9 8
#define D10 9

#define SWITCH_ON D1
#define SWITCH_MODE D2
#define SWITCH_LUM D6

#define STATUS_LED D0

#define OUT_RED D4
#define OUT_GREEN D5
#define OUT_BLUE D10

#define MODE_WHITE 0
#define MODE_COLOUR 1
#define MODE_FADING 2

#define RED_TO_BLUE 0
#define BLUE_TO_GREEN 1
#define GREEN_TO_RED 2

boolean switchOn = false;
int activeMode = MODE_WHITE;

// brightness
int brightness = 0;

// color picking
int previousSwitchColState = 0;
bool colorPicking = false;
int colorPickingCounter = 0;
int colorPickingCurrentTransition = 0;

// debounce buttons
int lastSwitchOnPressed = 0;
int lastSwitchModePressed = 0;
int lastSwitchColPressed = 0;

// white color mode
int whiteColorModeRed = 255;
int whiteColorModeGreen = 200;
int whiteColorModeBlue = 200;

// single color mode
int singleColorModeRed = 127;
int singleColorModeGreen = 127;
int singleColorModeBlue = 0;

// fading color mode
int fadingSpeed = 50;
int currentTransition = RED_TO_BLUE;
int fadingCounter = 0;
int fadingColorModeRed = 255;
int fadingColorModeGreen = 0;
int fadingColorModeBlue = 0;

void IRAM_ATTR onSwitchOnChanged()
{
  int millisNow = millis();
  bool ignore = millisNow - lastSwitchOnPressed < 50;
  lastSwitchOnPressed = millisNow;
  if (ignore)
    return;

  switchOn = !switchOn;
  Serial.println("Switch on changed: Now " + String(switchOn));
}

void IRAM_ATTR onSwitchModeChanged()
{
  int millisNow = millis();
  bool ignore = millisNow - lastSwitchModePressed < 50;
  lastSwitchModePressed = millisNow;
  if (ignore)
    return;

  activeMode = (activeMode + 1) % 3;
  Serial.println("Switch mode changed: Now " + String(activeMode));
}

void IRAM_ATTR onSwitchColChanged()
{
  int millisNow = millis();
  bool ignore = millisNow - lastSwitchColPressed < 50;
  lastSwitchColPressed = millisNow;
  if (ignore)
    return;

  int switchColState = digitalRead(SWITCH_LUM);
  bool risingEdge = previousSwitchColState == 0 && switchColState == 1;
  bool fallingEdge = previousSwitchColState == 1 && switchColState == 0;
  Serial.println("Switch col previous: " + String(previousSwitchColState) + ", current: " + String(switchColState));
  previousSwitchColState = switchColState;

  if (risingEdge)
  {
    colorPicking = true;
    fadingSpeed = 5;
  }
  if (fallingEdge)
  {
    colorPicking = false;
    fadingSpeed = 50;
  }
}

void switchOff()
{
  analogWrite(OUT_RED, 0);
  analogWrite(OUT_GREEN, 0);
  analogWrite(OUT_BLUE, 0);
}

void setup()
{
  Serial.begin(115200);
  delay(10000);
  Serial.println("Start");

  pinMode(SWITCH_ON, INPUT);
  attachInterrupt(SWITCH_ON, onSwitchOnChanged, RISING);

  pinMode(SWITCH_MODE, INPUT);
  attachInterrupt(SWITCH_MODE, onSwitchModeChanged, RISING);

  pinMode(SWITCH_LUM, INPUT);
  attachInterrupt(SWITCH_LUM, onSwitchColChanged, CHANGE);

  pinMode(STATUS_LED, OUTPUT);

  pinMode(OUT_RED, OUTPUT);
  pinMode(OUT_GREEN, OUTPUT);
  pinMode(OUT_BLUE, OUTPUT);

  // flash status led
  for (int i = 0; i < 5; i++)
  {
    digitalWrite(STATUS_LED, HIGH);
    delay(50);
    digitalWrite(STATUS_LED, LOW);
    delay(50);
  }
}

void loop()
{
  delay(fadingSpeed);
  Serial.println("Switch on: " + String(switchOn) + ", mode: " + String(activeMode));

  if (!switchOn)
  {
    switchOff();
    return;
  }

  // bool colorPicking = digitalRead(SWITCH_LUM) == 1;
  if (colorPicking)
  {
    Serial.println("Color picking");
    switch (colorPickingCurrentTransition)
    {
    case RED_TO_BLUE:
    {
      singleColorModeRed = 255 - colorPickingCounter;
      singleColorModeGreen = 0;
      singleColorModeBlue = colorPickingCounter;
      break;
    }

    case BLUE_TO_GREEN:
    {
      singleColorModeRed = 0;
      singleColorModeGreen = colorPickingCounter;
      singleColorModeBlue = 255 - colorPickingCounter;
      break;
    }
    case GREEN_TO_RED:
    {
      singleColorModeRed = colorPickingCounter;
      singleColorModeGreen = 255 - colorPickingCounter;
      singleColorModeBlue = 0;
      break;
    }
    default:
      break;
    }

    colorPickingCounter++;
    if (colorPickingCounter > 255)
    {
      colorPickingCounter = 0;
      colorPickingCurrentTransition = (colorPickingCurrentTransition + 1) % 3;
    }
  }

  switch (activeMode)
  {
  case MODE_WHITE:
  {
    analogWrite(OUT_RED, whiteColorModeRed);
    analogWrite(OUT_GREEN, whiteColorModeGreen);
    analogWrite(OUT_BLUE, whiteColorModeBlue);
    break;
  }
  case MODE_COLOUR:
  {
    analogWrite(OUT_RED, singleColorModeRed);
    analogWrite(OUT_GREEN, singleColorModeGreen);
    analogWrite(OUT_BLUE, singleColorModeBlue);
    break;
  }
  case MODE_FADING:
  {
    switch (currentTransition)
    {
    case RED_TO_BLUE:
      fadingColorModeRed = 255 - fadingCounter;
      fadingColorModeGreen = 0;
      fadingColorModeBlue = fadingCounter;
      break;

    case BLUE_TO_GREEN:
      fadingColorModeRed = 0;
      fadingColorModeGreen = fadingCounter;
      fadingColorModeBlue = 255 - fadingCounter;
      break;

    case GREEN_TO_RED:
      fadingColorModeRed = fadingCounter;
      fadingColorModeGreen = 255 - fadingCounter;
      fadingColorModeBlue = 0;
      break;

    default:
      break;
    }

    fadingCounter++;
    if (fadingCounter > 255)
    {
      fadingCounter = 0;
      currentTransition = (currentTransition + 1) % 3;
    }
    analogWrite(OUT_RED, fadingColorModeRed);
    analogWrite(OUT_GREEN, fadingColorModeGreen);
    analogWrite(OUT_BLUE, fadingColorModeBlue);

    break;
  }
  default:
    break;
  }
}
