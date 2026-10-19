# Data

This directory holds the backend-aligned sensor CSV schema and the intended place for raw and processed project datasets. The committed CSV template is **header-only**; it is not a dataset and contains no measured rows.

## Contents and conventions

- `raw/hardware/vikasa_sensor_data_template.csv` — canonical 24-column header for the backend record format.
- `raw/hardware/` — intended location for raw observations from the Vikāsa hardware, using dated/session-labelled filenames; do not overwrite the template with collected data.
- `processed/` — reserve for data that has been cleaned/transformed, with the processing method recorded.
- `sample/` and `templates/` — optional future sample/template assets; create these when there is a concrete, documented need.

The current repository does not include a final validated training dataset. The separate USB serial-to-Excel workflow is documented in the repository's [`Data-Logger/`](../../Data-Logger/README.md) folder.

## Sensor schema rules

- Preserve raw red/NIR, dark, and reference ADC counts separately from calibrated reflectance.
- Calculate calibrated NDVI only from calibrated reflectance values. A ratio derived from raw ADC counts is an exploratory indicator, not validated NDVI.
- Use `null`/blank for unmeasured fields; do not replace unknown values with zero.
- Each observation should carry traceable identifiers and timestamp plus provenance, operator/session, species, label evidence, and calibration notes wherever applicable.
- Do not combine external spectroscopy, environmental-only, and Vikāsa sensor data until units, source, acquisition conditions, calibration, and label semantics are shown compatible.

## Audit and ML readiness

From `Vikaasa-Project/`, run:

```bash
python scripts/audit_datasets.py --data-dir Data --output docs/dataset_audit.md --include-templates
```

This writes an audit report to `docs/dataset_audit.md`; it does not modify the source CSVs. Review [`../docs/dataset_eligibility.md`](../docs/dataset_eligibility.md), [`../docs/ml_readiness.md`](../docs/ml_readiness.md), and [`../docs/data_collection_protocol.md`](../docs/data_collection_protocol.md) before training or publishing results.
