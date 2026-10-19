# Standalone Arduino Sketch (Reference)

`Vikaasa_Final.ino` is a single-file ESP32-style Arduino sketch supplied separately from the modular PlatformIO project. It reads analog light and soil sensors, samples red/NIR responses using an ADS1115, reads temperature/humidity from a DHT22, displays values on an SH1106 OLED, and periodically attempts an HTTP upload to the Flask sensor-data endpoint.

## Configure before use

Edit the `WIFI_SSID`, `WIFI_PASSWORD`, and `API_URL` constants in the sketch. The checked-in file contains placeholders only. Replace `YOUR_COMPUTER_LAN_IP` with the computer's LAN address reachable from the board, and make sure the Flask server is reachable on that network. Do not enter credentials into a public commit.

The sketch uses Adafruit ADS1X15, DHT sensor library, Adafruit GFX, Adafruit SH110X, and the platform's Wi-Fi/HTTP/time libraries. Confirm the pin definitions (`A2`, `A3`, `D0`, `D1`, `D3`, SDA/SCL) against the actual board; pin labels differ between boards.

## How this differs from PlatformIO firmware

This is an independent prototype, not a renamed copy of the modular firmware. It has its own pin mapping, timing, simplified JSON payload, OLED behaviour, upload loop, and threshold-based status output. The primary backend schema and the modular firmware have stricter measurement semantics. Therefore, **do not assume this sketch's payload is compatible with every current API validation rule or that its health label is scientifically validated**. The modular implementation under `../PlatformIO/` is the primary firmware documented by the project's CI.

## Measurement limitations

The sketch computes an NDVI-style ratio from raw ADS1115 values. The values are not calibrated reflectance and the threshold is a prototype rule, not a validated plant-health classifier. No physical board test or calibration result is implied by including this file. Use the dataset logger sketches under [`../../../Data-Logger/firmware/`](../../../Data-Logger/firmware/) for the separate serial-to-Excel collection workflow.
