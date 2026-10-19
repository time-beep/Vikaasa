#   Vikāsa 🌱

**A plant-health monitoring project by Team Nexovate, Bangalore Institute of Technology.** Vikāsa explores optical plant-stress observation using red/NIR sensor readings alongside environmental measurements.

The team won **third place at Dakshinayasa Darshini**, hosted by Jyoti Institute of Technology.

## What's New in This Version?

Compared with the earlier CropSense version, Vikāsa expands the project with three major additions:

### 1. 🖨️ 3D-Printed Leaf Mount
- Added a 3D-printable leaf-mount model to help position the sensing hardware consistently during plant measurements.
- Provides a physical mounting solution for the sensor setup and supports repeatable data collection.

### 2. 📊 Sensor Data Logger
- Added a Python-based data logger to collect sensor readings over a serial connection.
- Records environmental measurements in an Excel workbook, with support for labelling observations by plant condition.
- Makes it easier to organise collected readings for further analysis and dataset preparation.

### 3. 🧠 Machine-Learning Workflow and Pipeline
- Added a modular ML pipeline covering data validation, preprocessing, feature extraction, model training, evaluation, and model export.
- Includes supporting utilities and documentation for assessing dataset quality, calibration, label validity, and suitability for training.
- Establishes the foundation for developing and evaluating plant-health prediction models.

**Current status:** The ML workflow is available as a development foundation, but trained-model inference is not yet integrated into the live monitoring system. Physical sensor calibration and validation remain necessary.



## Project areas

- **[Data-Logger](Data-Logger/README.md)** — a serial data collection utility, compatible sensor-collector sketches, and a blank Excel workbook template.
- **[Vikaasa-Project](Vikaasa-Project/README.md)** — the main software, firmware, hardware notes, 3D model, ML utilities, data schema, and documentation.

## System at a glance

```text
Sensor node → Wi-Fi/HTTP → Flask API → memory / Supabase / Firebase storage → React dashboard
```

The source contains implementation and automated checks for the API, dashboard, and PlatformIO firmware. Physical hardware calibration and end-to-end device connectivity have not been verified by the repository's CI. The ML utilities are not deployed as live inference, and raw NDVI-style values are not calibrated plant-health measurements.

Read the [main project guide](Vikaasa-Project/README.md) for setup, build commands, component documentation, and current limitations. The project is licensed under MIT; see [`LICENSE`](LICENSE).
