#include "esphome.h"
using namespace esphome;

class MyCustomFloatOutput : public Component, public FloatOutput {
 public:
  void setup() override {
    // This will be called by App.setup()
    //pinMode(5, OUTPUT);
  }

  void write_state(float state) override {
    // state is the amount this output should be on, from 0.0 to 1.0
    // we need to convert it to an integer first
    //int value = state * 1024;
    //analogWrite(5, value);
  }
};

// Custom binary output, for exposing binary states
class MyCustomBinaryOutput : public Component, public BinaryOutput {
 public:
  void setup() override {
    // This will be called by App.setup()
    //pinMode(5, OUTPUT);
  }

  void write_state(bool state) override {
    //digitalWrite(5, state);
  }
};