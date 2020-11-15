#include <IRremote.h>
#include <WiFi.h>

gpio_num_t IR_REC_PIN = GPIO_NUM_27;
const int ledPin = GPIO_NUM_5;
const int ledFreq = 5000;
const int ledChannel = 0;
const int ledResolution = 8;

IRrecv irrecv(IR_REC_PIN);
decode_results results;
int program;
int brightness;
String input;


String map_ir_to_str(int ir) {
  switch (ir) {
    case 0xF700FF:
      return "brightness_up";
    case 0xF7807F:
      return "brightness_down";
    case 0xF740BF:
      return "off";
    case 0xF7C03F:
      return "on";
    case 0xF720DF:
      return "R";
    case 0xF710EF:
      return "R1";
    case 0xF730CF:
      return "R2";
    case 0xF708F7:
      return "R3";
    case 0xF728D7:
      return "R4";
    case 0xF7A05F:
      return "G";
    case 0xF7906F:
      return "G1";
    case 0xF7B04F:
      return "G2";
    case 0xF78877:
      return "G3";
    case 0xF7A857:
      return "G4";
    case 0xF7609F:
      return "B";
    case 0xF750AF:
      return "B1";
    case 0xF7708F:
      return "B2";
    case 0xF748B7:
      return "B3";
    case 0xF76897:
      return "B4";
    case 0xF7E01F:
      return "W";
    case 0xF7D02F:
      return "P1";
    case 0xF7F00F:
      return "P2";
    case 0xF7C837:
      return "P3";
    case 0xF7E817:
      return "P4";
    case 0xFFFFFFFF:
      return input;
    default:
      return "none";
  }
}

// the setup function runs once when you press reset or power the board
void setup() {
  // configure LED PWM functionalitites
  ledcSetup(ledChannel, ledFreq, ledResolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);

  Serial.begin(115200);
  irrecv.enableIRIn(); // Start the receiver
  
  WiFi.mode(WIFI_OFF);
  btStop();

  for(int i = 0; i <= 255; i++) {
    ledcWrite(ledChannel, i);
    delay(10);
  }

  ledcWrite(ledChannel, 0);
  
  //esp_sleep_enable_ext0_wakeup(IR_REC_PIN, 1);
  //print_wakeup_reason();
}

//Function that prints the reason by which ESP32 has been awaken from sleep
void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch(wakeup_reason)
  {
    case 1  : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case 2  : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case 3  : Serial.println("Wakeup caused by timer"); break;
    case 4  : Serial.println("Wakeup caused by touchpad"); break;
    case 5  : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.println("Wakeup was not caused by deep sleep"); break;
  }
}

// the loop function runs over and over again forever
void loop() {
  if (irrecv.decode(&results))
  {
    Serial.println(results.value, HEX);
    input = map_ir_to_str(results.value);
    if(input == "off") {
      ledcWrite(ledChannel, 0);
      //esp_deep_sleep_start();
    }
    if(input == "on") {
      ledcWrite(ledChannel, 255);
    }
    Serial.println(input);
    irrecv.resume();
  }
}
