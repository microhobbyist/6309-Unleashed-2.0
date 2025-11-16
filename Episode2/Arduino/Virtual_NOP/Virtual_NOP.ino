const uint8_t pinClock = 4;         // Clock output from Arduino to pin 38 of CPU.
const uint8_t pinE = 2;             // E clock phase from CPU (preceeds Q by 90°).
const uint8_t pinLED = 13;          // Typically pin 13, this is the onboard LED
const uint8_t pinAddr[16] = {22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37};
const uint8_t pinData[8]  = {38,39,40,41,42,43,44,45};

volatile uint16_t gAddress = 0;
volatile bool     gPrintPending = false;

void onChangeE()
{
  if(digitalRead(pinE))             // If E clock is HIGH, then...
  {
    digitalWrite(pinLED, HIGH);     // Turn on the internal LED.
    gAddress = readAddressBus();    // Read the Address Bus.
    gPrintPending = true;
  }
  else                              // If E clock is LOW, then...
  {
    digitalWrite(pinLED, LOW);      // Turn off the internal LED.
  }
}

static uint16_t readAddressBus()
{
  uint16_t addr = 0;
  for(uint8_t index = 0; index < 16; index++)
  {
    if(digitalRead(pinAddr[index]))
    {
      addr |= (1U << index);
    }
  }
  return addr;
}

static void writeDataBus(uint8_t value)
{
  for(uint8_t index = 0; index < 8; index++)
  {
    uint8_t bitValue = (value >> index) & 1U;
    digitalWrite(pinData[index],bitValue);
  }
}

void setup() 
{
  Serial.begin(115200);             // Setup Serial port @ 115200bps.

  // Set all 16 address pins as inputs.
  for (uint8_t index = 0; index < 16; index++) pinMode(pinAddr[index], INPUT);

  // Set all 8 data pins as outputs.
  for (uint8_t index = 0; index < 8; index++) pinMode(pinData[index], OUTPUT);
  // Output NOP (no operation) opcode on the databus.
  writeDataBus(0x12);

  pinMode(pinLED, OUTPUT);          // Set the on-board LED as output.
  digitalWrite(pinLED, LOW);        // Start with the LED turned off.

  pinMode(pinClock, OUTPUT);        // Set the clock pin as output.
  digitalWrite(pinClock, LOW);      // Start the clock in a low state.

  pinMode(pinE, INPUT);             // Set the E clock phase pin as input
  attachInterrupt(digitalPinToInterrupt(pinE), onChangeE, CHANGE);  // Each time there's a state change,
                                                                    // execute onChange procedure.
  Serial.println("Virtual NOP Tester - Arduino Mega Edition");
  Serial.println("-----------------------------------------");
  Serial.println();
}

void loop() 
{
  digitalWrite(pinClock, HIGH);     // Simple clock, high phase.
  uint32_t previousMillis = millis();
  do
  {
    if(gPrintPending)
    {
      noInterrupts();
      gPrintPending = false;        // Make sure we don't print more then once on the same clock phase
      uint16_t addr = gAddress;     // Make a local copy, in case gAddress changes mid print of address
      interrupts();
      // Print address in binary format
      for(int8_t index = 15; index >= 0; index--)
      {
        if((addr & (1 << index)) != 0)
        {
          Serial.print('1');
        }
        else
        {
          Serial.print('0');
        }
      }
      Serial.println();
    }
  }
  while(millis() - previousMillis < halfCycle);

  digitalWrite(pinClock, LOW);      // Simple clock, low phase.
  delay(halfCycle);                 // Wait 25ms.
}