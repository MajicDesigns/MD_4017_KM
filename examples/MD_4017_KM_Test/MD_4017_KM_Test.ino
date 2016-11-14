#include <MD_4017_KM.h>

// Hardware connections and pins
#define NUM_KEYS  9
#define CLK_PIN   5
#define RST_PIN   6
#define KEY_PIN   7

MD_4017_KM  KM(NUM_KEYS, CLK_PIN, KEY_PIN, RST_PIN);

void setup(void)
{
  Serial.begin(57600);
  Serial.println("[MD_4017_KM Test]");
  Serial.println("Pressed keys are echoed to the serial monitor");
  
  KM.begin();
}

void loop(void)
{
  int16_t k = KEY_NONE;
  bool  bFirst = true;
 
  do 
  {
    k = KM.read(k+1);
    
    if (k != KEY_NONE)
    {
      if (bFirst)
      {
        Serial.println();
        bFirst = false;
      }
      Serial.print(k);
      Serial.print(" ");
    }
  } while (k != KEY_NONE);
}