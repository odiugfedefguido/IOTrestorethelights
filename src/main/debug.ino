// 0 for disable the debug and 1 for active it 
#define DEBUG 1

// functions to show the debug messages in the serial line
void debug(char* message) 
{
  if (DEBUG)
  {
    Serial.println(message);
  }
}

void debug(const __FlashStringHelper *message) {
  if (DEBUG)
  {
    Serial.println(message);
  }
}

void debug(const Printable& message) {
  if (DEBUG)
  {
    Serial.println(message);
  }
}

void debug(const StringSumHelper& message) {
  if (DEBUG)
  {
    Serial.println(message);
  }
}