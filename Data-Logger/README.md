# Vikaasa Data Logger

This utility collects labelled sensor observations from a microcontroller over USB serial and saves each operator-confirmed reading to an Excel workbook. It is separate from the live Flask/dashboard data path.

## Files

- `vikaasa_data_logger.py` — Python serial reader, condition prompt, NDVI-style index field, and Excel append/save loop.
- `vikaasa_data_logger.xlsx` — blank workbook template. The sheet is `Vikaasa Data`; the logger writes new observations here. Keep a clean copy before collecting real measurements.
- `firmware/vikaasa_dataset_collector_oled.ino` — preferred collection sketch; shows live readings on a 128×64 SH1106 OLED and emits the serial record expected by the logger.
- `firmware/vikaasa_dataset_collector_basic.ino` — non-OLED alternative with the same serial data format.
- `requirements.txt` — Python dependencies for serial access and workbook writing.

## Data flow

```text
Sensor collector sketch → USB serial (115200 baud) → Python logger → Excel workbook
```

The sketches send one comma-separated line at a time:

```text
DATA,temp,humidity,ldr,soil,red,nir,ndvi
```

The Python script accepts this eight-field format. It keeps the latest complete record in memory; when the operator types a condition/characteristic and presses Enter, it appends that snapshot with a timestamp and saves the workbook immediately. Non-`DATA` status lines are ignored. The program stops when `q` is entered or with Ctrl+C.

## Setup

1. Install Arduino IDE and the libraries used by the selected sketch: **Adafruit ADS1X15**, **DHT sensor library**; for the OLED variant also install **Adafruit GFX** and **Adafruit SH110X**.
2. Open one sketch from `firmware/` and confirm its pin definitions match the actual board/wiring. These are examples for the project's intended sensor setup, not a universal pinout.
3. Select the board and COM/serial port in Arduino IDE, compile, upload, and open Serial Monitor at **115200 baud**. Close Serial Monitor before starting Python, because the port can usually be opened by only one program at a time.
4. In this folder, install dependencies:

   ```bash
   python -m pip install -r requirements.txt
   ```

5. Keep `vikaasa_data_logger.py` beside `vikaasa_data_logger.xlsx`. Run:

   ```bash
   python vikaasa_data_logger.py
   ```

6. Select the serial port when prompted. Wait for complete `DATA` messages, type a condition label such as `healthy leaf` or `water-stressed`, and press Enter to save the latest snapshot. Type `q` to exit.

## Workbook columns

`Reading #`, `Timestamp`, `Temperature (°C)`, `Humidity (%)`, `LDR (raw)`, `Soil Moisture (raw)`, `RED LED (raw)`, `NIR LED (raw)`, `NDVI`, and `Condition / Characteristic`.

The condition is entered by the operator; the software does not verify the biological label. Record species, treatment, operator/session, lighting, calibration notes, and other required metadata in a separate collection log or extend the schema deliberately before analysing results.

## Measurement cautions

- The red/NIR values are raw ADC counts. The reported index uses `(NIR - Red) / (NIR + Red)` and is a **raw-count NDVI-style estimate**, not validated NDVI unless the sensor pipeline has been calibrated appropriately.
- In the OLED and basic sketches, an invalid zero-denominator calculation is currently printed as `0.0000` for compatibility with the numeric CSV-like line. Treat zero at that boundary as potentially invalid, not as a measured NDVI of zero.
- Sensor readings are captured continuously but the record saved is the latest complete reading available when Enter is pressed; it is not guaranteed to be the exact instant the operator typed the label.
- No measured workbook is bundled. Keep collected observations in separate, dated files and verify them before sharing or using them for ML.

## Troubleshooting

- **No port detected:** reconnect the board, check its USB cable/driver, and verify it appears in Device Manager or the operating system's serial-port list.
- **Port busy:** close Arduino Serial Monitor and any other serial application.
- **No rows saved:** wait for the complete `DATA,...` output and verify baud rate is 115200.
- **Missing OLED/ADS1115:** check I²C wiring and addresses (`0x3C` OLED, `0x48` ADS1115 in the provided sketches).
- **Workbook error:** do not rename the sheet or column headers unless you also update the logger's validation code.
