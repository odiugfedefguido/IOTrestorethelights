#define DEBUG 0

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