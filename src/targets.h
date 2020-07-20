#ifdef ARDUINO_TARGET
#define AD8318_PIN A3
#define CORRECTION 4
#define ADC_CORRECTION 0
#endif

#ifdef TTGO_LORA_V2_TARGET
#define AD8318_PIN 34
#define CORRECTION -1
#define ADC_CORRECTION 290
#endif