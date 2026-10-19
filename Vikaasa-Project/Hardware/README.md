# Hardware Notes

This folder is the hardware documentation entry point for the Vikāsa sensing prototype. It links the sensor concept, firmware variants, wiring/setup notes, and 3D-printed mounting asset. The repository does not yet provide a complete, validated production schematic or a full field-calibration report, so use these notes as integration guidance rather than a certified assembly procedure.

## Sensing and supporting parts

- **Optical channel:** red and near-infrared (NIR) illumination with a photodetector/analogue front end. The modular firmware reads optical channels through an ADS1115 ADC.
- **Temperature and humidity:** DHT22.
- **Soil moisture:** analogue sensor input; readings are raw ADC values until dry/wet reference points are calibrated.
- **Ambient light:** LDR/analogue input; raw ADC values are not lux without calibration.
- **Display on prototype variants:** 128×64 SH1106-compatible OLED. It is not used by every firmware path.
- **Mount:** [`../3D-Models/Leaf_Mount.stl`](../3D-Models/README.md), an unvalidated prototype for positioning a sensor near a leaf/stem.

## Where to check connections

- [`../docs/hardware_setup.md`](../docs/hardware_setup.md) — preliminary setup and wiring notes.
- [`../Firmware/PlatformIO/include/config.h`](../Firmware/PlatformIO/include/config.h) — pin/channel definitions for the modular PlatformIO firmware.
- [`../Firmware/Arduino_Standalone/Vikaasa_Final.ino`](../Firmware/Arduino_Standalone/Vikaasa_Final.ino) — a separate, single-file prototype with its own pin mapping.
- [`../../Data-Logger/firmware/`](../../Data-Logger/firmware/) — sketches for the independent USB-serial data collection workflow; see the [logger README](../../Data-Logger/README.md) for setup.

Do not assume pin names are portable between boards or firmware variants. Verify the target board, electrical levels, current limits, ADC channels, I²C addresses, sensor spacing, analogue-front-end behaviour, and power design before use.

## Validation status

The code and prototype mount are present in the repository, but their inclusion does not prove repeatable optical measurements, calibrated NDVI, mechanical fit, or field accuracy. Record changes to wiring, gains, LED current, mounting geometry, and calibration references with the dataset so readings can be traced to a particular hardware setup.
