// Wemos D1 Mini Pins
#define D0 16
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15

#define SWITCH_ON D7
#define SWITCH_COL D6
#define SWITCH_LUM D5

#define STATUS_LED D8

#define OUT_RED D1
#define OUT_GREEN D2
#define OUT_BLUE D3

boolean switchOn = false;
int brightness = 0;

ICACHE_RAM_ATTR void onSwitchOnChanged()
{
    Serial.println("Hello from interrupt");
    switchOn = !switchOn;
    digitalWrite(STATUS_LED, switchOn);
}

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        delay(50);
    Serial.println("Serial port initialized");

    pinMode(SWITCH_ON, INPUT);
    attachInterrupt(SWITCH_ON, onSwitchOnChanged, RISING);

    pinMode(SWITCH_COL, INPUT);
    pinMode(SWITCH_LUM, INPUT);

    pinMode(STATUS_LED, OUTPUT);

    pinMode(OUT_RED, OUTPUT);
    pinMode(OUT_GREEN, OUTPUT);
    pinMode(OUT_BLUE, OUTPUT);
}

void loop()
{
    // fade from red to blue
    for (int i = 0; i < 255; i++)
    {
        analogWrite(OUT_RED, 255 - i);
        analogWrite(OUT_GREEN, 0);
        analogWrite(OUT_BLUE, i);
        delay(10);
    }

    // fade from blue to green
    for (int i = 0; i < 255; i++)
    {
        analogWrite(OUT_RED, 0);
        analogWrite(OUT_GREEN, i);
        analogWrite(OUT_BLUE, 255 - i);
        delay(10);
    }

    // fade from green to red
    for (int i = 0; i < 255; i++)
    {
        analogWrite(OUT_RED, i);
        analogWrite(OUT_GREEN, 255 - i);
        analogWrite(OUT_BLUE, 0);
        delay(10);
    }
}
