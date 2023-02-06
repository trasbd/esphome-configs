#include "esphome.h"
#include "ble_binary_output.h"
//#include "esphome/components/ble_client/output/ble_binary_output.h"
//#include ".esphome/build/doorbell/src/esphome.h"
using namespace esphome;
namespace espbt = esphome::esp32_ble_tracker;

class myBLEOutput : public esphome::ble_client::BLEBinaryOutput, public FloatOutput {
public:
  int red;
  int green;
  int blue;
  void setup() override { 
    require_response_ = false;
    this->red = 255;
    green = 0;
    this->blue = 0;
    set_service_uuid128((unsigned char*)"FFF0");
    set_char_uuid128((unsigned char*)"FFF3");
    setup();
    
  }
  void write_state_to_device()
  {
    if (this->client_state_ != espbt::ClientState::ESTABLISHED)
    {
      ESP_LOGW("ble_client", "[%s] Not connected to BLE client.  State update can not be written.",
               this->char_uuid_.to_string().c_str());
      return;
    }

    auto *chr = this->parent()->get_characteristic(this->service_uuid_, this->char_uuid_);
    if (chr == nullptr)
    {
      ESP_LOGW("ble_client", "[%s] Characteristic not found.  State update can not be written.",
               this->char_uuid_.to_string().c_str());
      return;
    }
    // # List of bytes to write.
    //          #                         R       G     B
    //          value: [0x7e, 00, 05, 03, 0x00, 0x00, 255, 00, 0xef]

    //std::vector<uint8_t> state_as_uint {0x7e, 00, 05, 03, this->red, this->green, this->blue, 00, 0xef};
    std::vector<uint8_t> state_as_uint {0x7e, 00, 05, 03, 255, 0, 255, 00, 0xef};
    //uint8_t state_as_uint = (uint8_t)stateVector;
    ESP_LOGV(esphome::ble_client::TAG, "[%s] Write State: %d", this->char_uuid_.to_string().c_str(), state_as_uint);
    if (this->require_response_)
    {
      chr->write_value(&(state_as_uint[0]), sizeof(state_as_uint), ESP_GATT_WRITE_TYPE_RSP);
    }
    else
    {
      chr->write_value(&(state_as_uint[0]), sizeof(state_as_uint), ESP_GATT_WRITE_TYPE_NO_RSP);
    }
  }
};

class myBLEOutputRed : public myBLEOutput
{
  public:
    void write_state(float state) override { this->red = (state*255); write_state_to_device();}
};

class myBLEOutputGreen : public myBLEOutput
{
  public:
    void write_state(float state) override { green = (state*255); write_state_to_device();}
};

class myBLEOutputBlue : public myBLEOutput
{
  public:
    void write_state(float state) override { blue = (state*255); write_state_to_device();}
};