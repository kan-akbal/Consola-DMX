#include <DmxMaster.h>
#include <Adafruit_NeoPixel.h>

#define Tira 6  
#define NLED 60  

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NLED, Tira, NEO_GRB + NEO_KHZ800);

const int ledRedPin = 9;
const int ledGreenPin = 10;
const int ledBluePin = 11;

int red, green, blue, fader, del, preset;




void serialread();

void neopixel (int f, int r, int g, int b, int d, int p);

void dmx(int f, int r, int g, int b, int d, int p);

void led(int r, int g, int b);



void setup() 
{
  

  Serial.begin(9600);

  pinMode(ledRedPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  pinMode(ledBluePin, OUTPUT);

  strip.begin(); 
  strip.clear();

  DmxMaster.maxChannel(512);     
  DmxMaster.usePin(3);   

}

void loop() 
{
  int r = red;
  int g = green;
  int b = blue;
  int f = fader;
  int d = del;
  int p = preset;

  serialread();

  led(r, g, b);

  neopixel(f, r, g, b, d, p);

  dmx(f, r, g, b, d, p);


}

void  led(int r, int g, int b)
{

  analogWrite(ledRedPin, r);
  analogWrite(ledGreenPin, g);
  analogWrite(ledBluePin, b);

}

void dmx(int f, int r, int g, int b, int d, int p)
{

  for (int i = 1; i < 512; i = i+16)
  {
    DmxMaster.write(i, f);
    DmxMaster.write(i+1, r);
    DmxMaster.write(i+2, g);
    DmxMaster.write(i+3, b);
    DmxMaster.write(i+4, d);
    DmxMaster.write(i+5, p);  
  }
}

void neopixel (int f, int r, int g, int b, int d, int p)
{ 

  if(d > 0)
  {
    d = map (d, 1, 255, 1300, 50);

    
    strip.setBrightness(f);
    strip.show();
    delay(50); 
    strip.clear();
    strip.show();    
    delay(d);

  }
  else
  {
    strip.setBrightness(f);
  }
  if(p > 0)
  {
    strip.rainbow(0,1,p);
    strip.show();
  } 
  else
  {
    for (int i = 0; i < NLED; i++)
    {
      strip.setPixelColor(i, r, g, b);  
    }
    strip.show();
  }

}

void serialread()
{

if (Serial.available()) {
  String data = Serial.readStringUntil('\n');
  char str[data.length() + 1];
  strcpy(str, data.c_str());

  char* token = strtok(str, ",");
  int values[6];
  int numValues = 0;

  while (token != NULL && numValues < 6) {
    values[numValues] = atoi(token);
    token = strtok(NULL, ",");
    numValues++;
  }

  if (numValues == 6) {
      
    fader  = values[0];
    red    = values[1];
    green  = values[2];
    blue   = values[3];
    del    = values[4];
    preset = values[5];
  }
}

}
