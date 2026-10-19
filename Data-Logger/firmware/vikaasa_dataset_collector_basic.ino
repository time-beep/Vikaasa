#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <DHT.h>
#include <math.h>

// ============================================================
// VIKAASA DATASET COLLECTION FIRMWARE
// Sends one CSV-style DATA line over USB Serial for the Python
// Vikāsa_data_logger_updated.py script.
//
// Serial format:
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
#define ADS_ADDR      0x48

const unsigned long SERIAL_BAUD = 115200;
const unsigned long SAMPLE_INTERVAL_MS = 2000;

// Optical sensor timing
const unsigned long LED_SETTLE_MS = 100;
const unsigned long LED_OFF_SETTLE_MS = 50;
const int ADS_SAMPLES = 10;

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

// Average ADS1115 raw readings from the requested channel.
int readADSaverage(uint8_t channel) {
  long total = 0;

  for (int i = 0; i < ADS_SAMPLES; i++) {
    total += ads.readADC_SingleEnded(channel);
    delay(5);
  }

  return (int)(total / ADS_SAMPLES);
}

// Turn on only one LED at a time and read its corresponding ADS channel.
void readOpticalSensors() {
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(NIR_LED_PIN, LOW);
  delay(LED_OFF_SETTLE_MS);

  // RED LED -> ADS1115 A0
  digitalWrite(RED_LED_PIN, HIGH);
  delay(LED_SETTLE_MS);
  redRaw = readADSaverage(0);
  digitalWrite(RED_LED_PIN, LOW);
  delay(LED_OFF_SETTLE_MS);

  // NIR LED -> ADS1115 A1
  digitalWrite(NIR_LED_PIN, HIGH);
  delay(LED_SETTLE_MS);
  nirRaw = readADSaverage(1);
  digitalWrite(NIR_LED_PIN, LOW);
  delay(LED_OFF_SETTLE_MS);
}

// Calculate NDVI-style index from the red and NIR readings.
// Raw ADC counts are not calibrated reflectance, so treat this as
// a preliminary index until the optical readings are calibrated.
float calculateNDVI(int red, int nir) {
  long denominator = (long)nir + (long)red;

  if (denominator <= 0) {
    return NAN;
  }

  return ((float)nir - (float)red) / (float)denominator;
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  delay(500);

  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(NIR_LED_PIN, OUTPUT);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(NIR_LED_PIN, LOW);

  pinMode(LDR_PIN, INPUT);
  pinMode(SOIL_PIN, INPUT);

  Wire.begin(SDA_PIN, SCL_PIN);

  if (!ads.begin(ADS_ADDR)) {
    // Keep this error visible for troubleshooting. The Python logger
    // ignores non-DATA lines.
    Serial.println("ERROR: ADS1115 not detected at address 0x48.");
    while (true) {
      digitalWrite(RED_LED_PIN, LOW);
      digitalWrite(NIR_LED_PIN, LOW);
      delay(1000);
    }
  }

  ads.setGain(GAIN_ONE);
  ads.setDataRate(RATE_ADS1115_860SPS);

  dht.begin();

  Serial.println("Vikaasa dataset logger firmware ready.");
  Serial.println("Expected output: DATA,temp,humidity,ldr,soil,red,nir,ndvi");
}

void loop() {
  if (millis() - lastSampleMs < SAMPLE_INTERVAL_MS) {
    return;
  }
  lastSampleMs = millis();

  // Environmental sensors
  ldrRaw = analogRead(LDR_PIN);
  soilRaw = analogRead(SOIL_PIN);

  // Optical measurements, with RED and NIR measured separately
  readOpticalSensors();

  // DHT22 measurements
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  // Do not send incomplete environmental readings.
  // The Python logger expects numeric values for every field.
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("WARNING: DHT22 reading invalid; sample skipped.");
    return;
  }

  ndvi = calculateNDVI(redRaw, nirRaw);

  // The Python logger accepts:
  // DATA,temp,humidity,ldr,soil,red,nir,ndvi
  // Keep each sample on one line and do not add labels between values.
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

  if (isnan(ndvi)) {
    // A zero denominator means NDVI cannot be calculated.
    // Send a numeric placeholder; see note below.
    Serial.println("0.0000");
  } else {
    Serial.println(ndvi, 4);
  }
}
