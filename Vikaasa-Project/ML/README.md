# Machine Learning Utilities

This package contains reusable data-quality, preprocessing, feature-engineering, training-interface, evaluation, and export utilities for future Vikāsa plant-stress modelling. It is not a deployed model service: there are no verified trained model weights, accuracy claims, or live inference calls from the firmware/Backend/dashboard.

## Modules

- `data_validation/` — checks expected schema/columns, missingness, numeric ranges, and potential confusion between raw ADC counts and calibrated values.
- `preprocessing/` — duplicate/time/plant/session filters, removal of rows without required calibrated reflectance, feature scaling, and leakage-aware dataset splitting helpers.
- `features/` — optical feature extraction (including reflectance/NDVI helper functions) and environmental feature preparation.
- `training/` — trainer configuration/interfaces and training-data readiness checks; optional model libraries may be needed for particular trainer classes.
- `evaluation/` — classification metrics, subgroup/NDVI stratification, calibration-curve, and report-generation utilities.
- `export/` — export interfaces/helpers for selected model formats; successful export depends on the model framework and installed optional tools.
- `models/` — model-related definitions and common structures; this directory does not imply that a production model artifact is included.

## Data prerequisites

Before supervised training, ensure the source data contains:

- traceable sample IDs and provenance (plant/session/operator/time/species where applicable);
- appropriately calibrated red/NIR reflectance (raw ADC values alone are not calibrated reflectance);
- verified labels and their evidence/source;
- environmental measurements collected under documented conditions;
- appropriate grouping or temporal splits that avoid leaking related readings across train and test sets.

See [`../docs/ml_workflow.md`](../docs/ml_workflow.md), [`../docs/ml_readiness.md`](../docs/ml_readiness.md), [`../docs/dataset_eligibility.md`](../docs/dataset_eligibility.md), and [`../Data/README.md`](../Data/README.md).

## Useful entry points

From `Vikaasa-Project/`, the dataset audit command is:

```bash
python scripts/audit_datasets.py --data-dir Data --output docs/dataset_audit.md
```

The Python modules use pandas/NumPy and may require additional ML dependencies for particular trainers/exporters. Install the relevant package before calling those optional paths. There is currently no single top-level command that trains, validates, selects, exports, and deploys a production model end to end.

## Scientific and implementation limits

- The existence of feature/training/evaluation code does not establish training data eligibility or model quality.
- The currently committed sensor CSV is a header-only template, not a measured dataset.
- Do not use raw-count NDVI estimates as calibrated NDVI or use unverified operator labels as confirmed plant-health ground truth.
- No ML inference is connected to the API response, dashboard, or ESP32 firmware in the current project.
