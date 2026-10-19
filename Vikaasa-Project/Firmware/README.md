# Firmware

This folder contains two distinct firmware paths. They should not be treated as interchangeable builds.

- [`PlatformIO/`](PlatformIO/README.md) — the primary, modular ESP32-C3 firmware. This is the implementation used by the repository's compile workflow and documented payload contract.
- [`Arduino_Standalone/`](Arduino_Standalone/README.md) — a separate single-file Arduino sketch retained as a reference for the integrated sensor/OLED/Wi-Fi/API prototype.

The separate dataset-collection sketches that emit the USB serial format for the Excel logger live under the repository's [`Data-Logger/firmware/`](../../Data-Logger/README.md) folder.

**Safety and validity:** configure credentials locally and never commit them. Compile success does not establish physical wiring, sensor calibration, API compatibility, or field performance. Review the specific subfolder README before selecting a firmware path.
