// Arduino Mega sketch to test ATF750C with real CPU integration
// Generates SLOW/FAST clks at 2MHz/4MHz (clean 50% duty)
// Monitors real CPU E to time bus accesses during E high
// Loops switches infinitely if pass; prints cycle count + halts on fail

// PLD input pins (Mega outputs)
#define PLD_SLOWCLK    11  // PIN 2: SLOWclk (Timer1 OC1A) - must stay for PWM
#define PLD_FASTCLK     5  // PIN 3: FASTclk (Timer3 OC3A) - must stay for PWM
#define PLD_RW          3  // PIN 4: R_W
#define PLD_SPEEDCS    13  // PIN 5: !SPEEDcs (active low)

// PLD bidirectional: D0 (Mega switches mode)
#define PLD_D0          7  // PIN 14: D0

// PLD output pins (Mega inputs) - reassigned to group better
#define PLD_CPUCLK      2  // PIN 15: CPU_CLK
#define PLD_SPEEDCURR   6  // PIN 16: SPEEDcurr
#define PLD_HALTOUT     9  // PIN 19: !HALTout (active low)
#define PLD_MRDYOUT    10  // PIN 20: !MRDYout (active low)

// Reset output to CPU (also connected to PLD PIN 6 reset_n)
#define CPU_RESET_OUT   8  // PIN 6: RESET

// Monitor real CPU E output (wire to CPU E pin)
#define CPU_E_MON      12  // PIN 1: E

// Frequencies (MHz range, 50% duty)
#define SLOW_FREQ_HZ 1UL  // 1 Hz
#define FAST_FREQ_HZ 2UL  // 2 Hz

// Delays (ms, scaled for faster clocks)
#define RESET_DELAY    10
#define SWITCH_DELAY    1   // Short for MHz; FSM settles in us

// Counter for successful full cycles (fast → slow without error)
unsigned long successful_cycles = 0;

void setup() {
  pinMode(13, OUTPUT);
  pinMode(PLD_SPEEDCS, OUTPUT);
  pinMode(PLD_RW, OUTPUT);
  pinMode(PLD_D0, OUTPUT);

  // Blink marker 1 - start of setup
  for (int i = 0; i < 3; i++) {
    digitalWrite(13, HIGH);
    digitalWrite(PLD_SPEEDCS, HIGH);
    delay(300);
    digitalWrite(13, LOW);
    digitalWrite(PLD_SPEEDCS, LOW);
    delay(300);
  }

  // Force CS low for 2 s - marker 2
  digitalWrite(PLD_SPEEDCS, LOW);
  digitalWrite(PLD_RW, LOW);
  digitalWrite(PLD_D0, HIGH);
  delay(2000);
  digitalWrite(PLD_SPEEDCS, HIGH);
  digitalWrite(PLD_RW, HIGH);
  digitalWrite(PLD_D0, LOW);

  // Blink marker 3 - end of setup, before loop
  for (int i = 0; i < 5; i++) {
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(100);
  }
}

void loop() {
  // Fast blink in loop to confirm we reached main loop
  digitalWrite(13, HIGH);
  delay(50);
  digitalWrite(13, LOW);
  delay(50);
}

// Sync access to real E cycle
void syncAccess() {
  while (digitalRead(CPU_E_MON) == HIGH);
  while (digitalRead(CPU_E_MON) == LOW);
  while (digitalRead(CPU_E_MON) == HIGH);
}

// Read from $FF01
int readSpeed() {
  pinMode(PLD_D0, INPUT);
  digitalWrite(PLD_RW, HIGH);
  digitalWrite(PLD_SPEEDCS, LOW);
  syncAccess();
  int val = digitalRead(PLD_D0);
  digitalWrite(PLD_SPEEDCS, HIGH);
  pinMode(PLD_D0, OUTPUT);
  return val;
}

// Write to $FF01
void writeSpeed(int val) {
  digitalWrite(PLD_D0, val);
  digitalWrite(PLD_RW, LOW);
  digitalWrite(PLD_SPEEDCS, LOW);
  syncAccess();
  digitalWrite(PLD_SPEEDCS, HIGH);
  digitalWrite(PLD_RW, HIGH);
}

// Check after switch
void checkSwitchComplete(int expectedSpeed) {
  delay(SWITCH_DELAY);
  int curr = digitalRead(PLD_SPEEDCURR);
  int mrody = digitalRead(PLD_MRDYOUT);
  int halt  = digitalRead(PLD_HALTOUT);

  if (curr != expectedSpeed || mrody != 1 || halt != 1) {
    Serial.print("FAIL at speed ");
    Serial.print(expectedSpeed);
    Serial.print(" after ");
    Serial.print(successful_cycles);
    Serial.print(" successful full cycles. ");
    Serial.print("curr=");
    Serial.print(curr);
    Serial.print(", MRDY=");
    Serial.print(mrody);
    Serial.print(", HALT=");
    Serial.println(halt);
    while (1);
  }
}

void testResetAndInitial() {
  digitalWrite(CPU_RESET_OUT, LOW);
  delay(RESET_DELAY);
  digitalWrite(CPU_RESET_OUT, HIGH);
  delay(RESET_DELAY);

  int initial = readSpeed();
  if (initial != 0) {
    Serial.println("FAIL: Initial speed != 0 after 0 cycles");
    while (1);
  }
}

void testSwitchToFast() {
  writeSpeed(1);
  checkSwitchComplete(1);

  int rdback = readSpeed();
  if (rdback != 1) {
    Serial.print("FAIL: Fast readback != 1 after ");
    Serial.print(successful_cycles);
    Serial.println(" successful full cycles");
    while (1);
  }
}

void testSwitchToSlow() {
  writeSpeed(0);
  checkSwitchComplete(0);

  int rdback = readSpeed();
  if (rdback != 0) {
    Serial.print("FAIL: Slow readback != 0 after ");
    Serial.print(successful_cycles);
    Serial.println(" successful full cycles");
    while (1);
  }
}