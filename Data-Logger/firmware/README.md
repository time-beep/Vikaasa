# Dataset Collector Sketches

Both sketches in this directory are designed for the separate USB serial-to-Excel data-logger workflow. They emit a single `DATA,temp,humidity,ldr,soil,red,nir,ndvi` line at 115200 baud; `../vikaasa_data_logger.py` accepts that format and records the latest complete reading when an operator enters a condition label.

- `vikaasa_dataset_collector_oled.ino` — preferred option when the SH1106 128×64 OLED is connected. Displays temperature/humidity, LDR, soil ADC, red/NIR raw values and a raw-count NDVI-style estimate.
- `vikaasa_dataset_collector_basic.ino` — same serial collection contract without the OLED dependency.

Libraries: Adafruit ADS1X15 and DHT sensor library for both; add Adafruit GFX and Adafruit SH110X for the OLED sketch. Confirm pin assignments and I²C addresses against the target board. These sketches are not the modular API-upload PlatformIO firmware.

The serial `NDVI` field is derived from raw red/NIR ADC counts and is not calibrated NDVI. Both examples currently print `0.0000` when the denominator is invalid, so interpret that boundary case with care. The operator-provided condition is not machine-verified ground truth.
