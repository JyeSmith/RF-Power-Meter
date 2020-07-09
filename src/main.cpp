#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define AD8318_PIN A3

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

uint16_t oledRefreshTime = 500; // ms
uint32_t previousSampleTime = 0;
uint16_t samples[SCREEN_WIDTH] = {0};
uint32_t samplingPeriod = 0;
uint16_t minSample = 65535;
uint16_t maxSample = 0;
uint32_t meanSample = 0;
uint8_t minSampledBm = 255;
uint8_t maxSampledBm = 0;
uint8_t meanSampledBm = 0;

void sampleAd8318()
{
  uint32_t startSampleingTime = millis();

  uint32_t tempReading = 0;
  for (int i = 0; i < SCREEN_WIDTH; i++)
  {
    tempReading = analogRead(AD8318_PIN);
    tempReading += analogRead(AD8318_PIN);
    tempReading += analogRead(AD8318_PIN);
    samples[i] = tempReading / 3;
  }

  samplingPeriod = millis() - startSampleingTime;

  minSample = 65535;
  maxSample = 0;
  meanSample = 0;

  for (int i = 0; i < SCREEN_WIDTH; i++)
  {

    float mV = (float)samples[i] * (5000.0 / 1023.0);
    float dBm = mV/-24.5 + 56.0;

    samples[i] = pow(10, (dBm / 10.0)); // dBm to mW

    if (samples[i] > maxSample)
    {
      maxSample = samples[i];
    }
    if (samples[i] < minSample)
    {
      minSample = samples[i];
    }

    meanSample += samples[i];

    // Serial.print(samples[i]);
    // Serial.print(",");
  }
  // Serial.println("");

  meanSample /= SCREEN_WIDTH;

  meanSampledBm = 10.0 * log10(meanSample);
  maxSampledBm = 10.0 * log10(maxSample);

  for (int i = 0; i < SCREEN_WIDTH; i++)
  {
    // samples[i] = map(samples[i], minSample, maxSample, 0, SCREEN_HEIGHT - 1 - 18);
    samples[i] = map(samples[i], minSample, maxSample, SCREEN_HEIGHT - 1 - 18, 0);
  }
}

void drawPlot()
{
  display.clearDisplay();

  for (int i = 0; i < SCREEN_WIDTH - 1; i++)
  {
    display.drawLine(i, samples[i] + 18, i + 1, samples[i + 1] + 18, WHITE);
  }

  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0, 0);

  display.print("Max:  ");
  display.print(maxSample);
  display.print(" mW, ");
  display.print(maxSampledBm);
  display.println(" dBm");

  display.print("Mean: ");
  display.print(meanSample);
  display.print(" mW, ");
  display.print(meanSampledBm);
  display.println(" dBm");

  display.display();
}
void setup()
{
  Serial.begin(115200);

  pinMode(AD8318_PIN, INPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  display.clearDisplay();
}

void loop()
{
  uint32_t now = millis();

  if (now > previousSampleTime + oledRefreshTime)
  {
    sampleAd8318();

    drawPlot();

    previousSampleTime = now;
  }
}