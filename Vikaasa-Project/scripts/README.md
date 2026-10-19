# Project Scripts

This directory contains helper scripts that support the Vikāsa data workflow. It currently includes `audit_datasets.py`, which scans CSV files beneath a chosen data directory and writes a Markdown report describing schema coverage, missingness, duplicates, labels, calibration/provenance flags, and suitability warnings.

## Run the audit

Run from the `Vikaasa-Project/` directory after installing the project dependencies:

```bash
python scripts/audit_datasets.py --data-dir Data --output docs/dataset_audit.md --include-templates
```

Use `--include-templates` to include the header-only Vikāsa CSV template. Without it, template-named files are skipped unless the script needs a template fallback because no other CSV was found. The script reads CSVs and writes the requested report; it does not change source CSV files.

## Interpretation

The output is a data-quality/provenance audit, not a model-training result. It cannot independently prove sensor calibration, label truth, measurement origin, or scientific validity. Review the source records and the protocol/readiness notes under [`../docs/`](../docs/README.md) before training or publishing any conclusions.
