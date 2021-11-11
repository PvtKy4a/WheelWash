#include <EncButton.h>
EncButton<EB_TICK, 8> mode1(INPUT_PULLUP);
EncButton<EB_TICK, 9> mode2(INPUT_PULLUP);
EncButton<EB_TICK, 10> mode3(INPUT_PULLUP);
EncButton<EB_TICK, 11> dry(INPUT_PULLUP);
EncButton<EB_TICK, 12> protect(INPUT_PULLUP);

#define pump 5
#define wheel_spin 4
#define air_valve 3
#define open_door_protect 2

void setup() {

  pinMode(pump, OUTPUT);
  pinMode(wheel_spin, OUTPUT);
  pinMode(air_valve, OUTPUT);
  pinMode(open_door_protect, OUTPUT);

}

void loop() {

  mode1.tick();
  mode2.tick();
  mode3.tick();
  dry.tick();
  protect.tick();

  static bool protect_flag = false;
  if (protect.hold()) protect_flag = true; else protect_flag = false;
  if (protect_flag == true) {
    digitalWrite(open_door_protect, LOW);
  } else digitalWrite(open_door_protect, HIGH);

  static uint32_t wash_timer;
  static uint32_t dry_timer;
  static bool wash_start = false;
  static bool dry_start = false;
  static bool mode_available = true;
  static uint16_t wash_time;
  if (protect_flag == true) {
    if (mode_available == true && mode1.held()) {
      mode_available = false;
      wash_time = 20000;
      digitalWrite(pump, HIGH);
      digitalWrite(wheel_spin, HIGH);
      wash_start = true;
      wash_timer = millis();
    }
    if (mode_available == true && mode2.held()) {
      mode_available = false;
      wash_time = 40000;
      digitalWrite(pump, HIGH);
      digitalWrite(wheel_spin, HIGH);
      wash_start = true;
      wash_timer = millis();
    }
    if (mode_available == true && mode3.held()) {
      mode_available = false;
      wash_time = 60000;
      digitalWrite(pump, HIGH);
      digitalWrite(wheel_spin, HIGH);
      wash_start = true;
      wash_timer = millis();
    }
    if (mode_available == true && dry.held()) {
      mode_available = false;
      digitalWrite(wheel_spin, HIGH);
      digitalWrite(air_valve, HIGH);
      dry_start = true;
      dry_timer = millis();
    }
    if (wash_start == true && millis() - wash_timer >= wash_time) {
      digitalWrite(pump, LOW);
      digitalWrite(air_valve, HIGH);
      wash_start = false;
      dry_start = true;
      dry_timer = millis();
    }
    if (dry_start == true && millis() - dry_timer >= 15000) {
      digitalWrite(wheel_spin, LOW);
      digitalWrite(air_valve, LOW);
      dry_start = false;
      mode_available = true;
    }
  } else if (wash_start == true || dry_start == true) {
    digitalWrite(pump, LOW);
    digitalWrite(wheel_spin, LOW);
    digitalWrite(air_valve, LOW);
    wash_start = false;
    dry_start = false;
    mode_available = true;
  }

}
