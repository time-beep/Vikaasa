# ESP32-C3 PlatformIO Firmware

This is the primary modular firmware implementation for the Vikāsa sensing node. It separates startup/orchestration, hardware acquisition, timestamps/reading IDs, Wi-Fi management, and API transport into `src/` and `include/` modules.

## Layout

- `platformio.ini` — environments and library dependencies.
- `boards/glyph_c3.json` — custom board definition for the GLYPH C3 board.
- `config/secrets.example.h` — safe configuration template. Copy to `config/secrets.h` for a local build; the real secrets file must remain untracked.
- `include/` — module interfaces, shared configuration, sensor data and transport declarations.
- `src/` — implementation and entry point (`main.cpp`).
- `examples/prototype_sensor_sketch.ino` — earlier prototype, retained for reference; not the active PlatformIO `src/main.cpp` build.
- `test/` — native unit tests for reading IDs, sensor data, and timestamps.
- `build_version.py` — build metadata helper referenced by PlatformIO.

## Prerequisites

- Python 3.11+ recommended for the repository and PlatformIO Core.
- PlatformIO Core (`python -m pip install platformio`) or the PlatformIO IDE extension.
- USB driver/serial access for the GLYPH C3 board when uploading/monitoring.

## Configure and build

Run commands from this directory:

```bash
# Copy the template; fill in Wi-Fi, API base URL, plant/session/operator IDs locally.
cp config/secrets.example.h config/secrets.h
pio run -e glyph_c3-dev
pio run -e glyph_c3
```

For PowerShell, use `Copy-Item config/secrets.example.h config/secrets.h`. `glyph_c3-dev` enables development mode; `glyph_c3` is the release configuration. The `API_BASE_URL` convention in this firmware is the server base with `/api` included, for example `http://192.168.1.10:5000/api` on a local network. Do not use `localhost` for a separate microcontroller: that would refer to the device itself.

Run the native test target when the required PlatformIO native toolchain is available:

```bash
pio test -e native-test
```

## Data and communication behaviour

The modular implementation is designed to read red/NIR optical channels via ADS1115, environmental values (DHT22, soil-moisture ADC, ambient-light ADC), apply the documented dark/reference acquisition logic, create reading identifiers/timestamps, and POST JSON to the Flask API. See [`../../docs/firmware.md`](../../docs/firmware.md) for the detailed payload, timestamp, retry, and configuration notes; see [`../../docs/system_architecture.md`](../../docs/system_architecture.md) for the wider flow.

## Important limitations

- The repo's CI compiles the release and development PlatformIO environments; it does not upload to a physical board or validate the electrical design.
- Optical reference calibration, amplifier characteristics, sensor geometry, field repeatability, and moisture/light calibration remain to be validated.
- Backend `ndvi` represents calibrated reflectance only. A raw-ADC-derived indicator is not equivalent to calibrated NDVI.
- Do not commit `config/secrets.h`, Wi-Fi credentials, tokens, or production service keys.
