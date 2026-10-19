#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Adafruit_ADS1X15.h>
#include <DHT.h>
#include <math.h>

// ============================================================
// VIKAASA DATA COLLECTION FIRMWARE
//
// Shows live readings on the OLED. The operator inspects the
// displayed values, then enters the ground-truth condition in
// the Python logger and presses ENTER to save one observation.
//
// Serial format expected by the Python logger:
// DATA,temp,humidity,ldr,soil,red,nir,ndvi
// ============================================================

// Pin configuration from the supplied Vikaasa firmware
#define LDR_PIN       A2
#define SOIL_PIN      A3
#define NIR_LED_PIN   D0
#define RED_LED_PIN   D1
#define DHT_PIN       D3
#define DHT_TYPE      DHT22

#define SDA_PIN       4
#define SCL_PIN       5
#define OLED_ADDR     0x3C
#define ADS_ADDR      0x48

const unsigned long SERIAL_BAUD = 115200;
const unsigned long SAMPLE_INTERVAL_MS = 2000;
const unsigned long LED_SETTLE_MS = 100;
const unsigned long LED_OFF_SETTLE_MS = 50;
const int ADS_SAMPLES = 10;

Adafruit_SH1106G display(128, 64, &Wire, -1);
Adafruit_ADS1115 ads;
DHT dht(DHT_PIN, DHT_TYPE);

unsigned long lastSampleMs = 0;

int ldrRaw = 0;
int soilRaw = 0;
int redRaw = 0;
int nirRaw = 0;
float temperature = NAN;
float humidity = NAN;
float ndvi = NAN;

int readADSaverage(uint8_t channel) {
  long total = 0;

  for (int i = 0; i < ADS_SAMPLES; i++) {
    total += ads.readADC_SingleEnded(channel);
    delay(5);
  }

  return (int)(total / ADS_SAMPLES);
}

void readOpticalSensors() {
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(NIR_LED_PIN, LOW);
  delay(LED_OFF_SETTLE_MS);

  // RED LED on alone -> ADS1115 A0
  digitalWrite(RED_LED_PIN, HIGH);
  delay(LED_SETTLE_MS);
  redRaw = readADSaverage(0);
  digitalWrite(RED_LED_PIN, LOW);
  delay(LED_OFF_SETTLE_MS);

  // NIR LED on alone -> ADS1115 A1
  digitalWrite(NIR_LED_PIN, HIGH);
  delay(LED_SETTLE_MS);
  nirRaw = readADSaverage(1);
  digitalWrite(NIR_LED_PIN, LOW);
  delay(LED_OFF_SETTLE_MS);
}

float calculateNDVI(int red, int nir) {
  long denominator = (long)nir + (long)red;

  if (denominator <= 0) {
    return NAN;
  }

  // Preliminary index from raw sensor counts; calibrate optical
  // measurements before interpreting this as scientific NDVI.
  return ((float)nir - (float)red) / (float)denominator;
}

void showStartupMessage(const char* line1, const char* line2) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println("Vikaasa Dataset");
  display.println(line1);
  display.println(line2);
  display.display();
}

void showReadings() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);

  display.println("Vikaasa - Data");

  display.print("T:");
  if (isnan(temperature)) display.print("--");
  else display.print(temperature, 1);
  display.print("C H:");
  if (isnan(humidity)) display.print("--");
  else display.print(humidity, 1);
  display.println("%");

  display.print("LDR: ");
  display.println(ldrRaw);

  display.print("Soil: ");
  display.println(soilRaw);

  display.print("RED: ");
  display.println(redRaw);

  display.print("NIR: ");
  display.println(nirRaw);

  display.print("NDVI: ");
  if (isnan(ndvi)) display.println("INVALID");
  else display.println(ndvi, 4);

  display.display();
}

void printSerialData() {
  // Keep this as a single CSV-style line so the Python reader can parse it.
  Serial.print("DATA,");
  Serial.print(temperature, 2);
  Serial.print(",");
  Serial.print(humidity, 2);
  Serial.print(",");
  Serial.print(ldrRaw);
  Serial.print(",");
  Serial.print(soilRaw);
  Serial.print(",");
  Serial.print(redRaw);
  Serial.print(",");
  Serial.print(nirRaw);
  Serial.print(",");

  if (isnan(ndvi)) Serial.println("0.0000");
  else Serial.println(ndvi, 4);
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  delay(300);

  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(NIR_LED_PIN, OUTPUT);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(NIR_LED_PIN, LOW);

  pinMode(LDR_PIN, INPUT);
  pinMode(SOIL_PIN, INPUT);

  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(400000);

  if (!display.begin(OLED_ADDR, true)) {
    Serial.println("ERROR: OLED initialization failed");
    while (true) delay(10);
  }

  if (!ads.begin(ADS_ADDR)) {
    showStartupMessage("ERROR:", "ADS1115 missing");
    Serial.println("ERROR: ADS1115 not detected at 0x48");
    while (true) delay(10);
  }

  ads.setGain(GAIN_ONE);
  ads.setDataRate(RATE_ADS1115_860SPS);
  dht.begin();

  showStartupMessage("Starting sensors...", "");
  Serial.println("Vikaasa dataset collector ready.");
  Serial.println("OLED shows live readings; Python records only when you press ENTER.");
  Serial.println("Format: DATA,temp,humidity,ldr,soil,red,nir,ndvi");
}

void loop() {
  if (millis() - lastSampleMs < SAMPLE_INTERVAL_MS) {
    return;
  }
  lastSampleMs = millis();

  ldrRaw = analogRead(LDR_PIN);
  soilRaw = analogRead(SOIL_PIN);

  readOpticalSensors();

  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  ndvi = calculateNDVI(redRaw, nirRaw);

  if (isnan(temperature) || isnan(humidity)) {
    showReadings();
    Serial.println("WARNING: DHT22 reading invalid; DATA sample skipped.");
    return;
  }

  showReadings();

  // Python's background serial reader keeps the latest complete DATA line.
  // The user reviews the OLED, types the condition in Python, and presses
  // ENTER; Python logs the latest complete reading it has received.
  printSerialData();
}
