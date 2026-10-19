# Documentation Index

This folder contains implementation notes, API contracts, architecture, calibration/collection guidance, data-quality audits, and ML-readiness notes. Read the current implementation docs alongside source code; proposals and historical reports are clearly distinguished from verified behaviour.

## System and API

- [`system_architecture.md`](system_architecture.md) — component roles, data flow, implemented choices, and unverified work.
- [`api.md`](api.md) — endpoint contracts, request/response shape, validation, error cases, and local testing.
- [`firebase.md`](firebase.md) — Firebase adapter configuration and integration notes.

## Firmware and hardware

- [`firmware.md`](firmware.md) — PlatformIO firmware architecture, sensor interfaces, JSON contract, timestamps, Wi-Fi and retry/configuration behaviour.
- [`hardware_setup.md`](hardware_setup.md) — preliminary wiring/configuration context; not a validated assembly manual.
- [`reports/firmware_final_report.md`](reports/firmware_final_report.md) — historical firmware implementation/build report; refer to current source and CI for present code.

## Data and ML

- [`data_collection_protocol.md`](data_collection_protocol.md) — proposed acquisition/label/provenance practices.
- [`dataset_audit.md`](dataset_audit.md) — generated/audited dataset-status notes; re-run the audit when data changes.
- [`dataset_eligibility.md`](dataset_eligibility.md) — current repository data inventory and eligibility limitations.
- [`ml_readiness.md`](ml_readiness.md) — prerequisites before model training.
- [`ml_workflow.md`](ml_workflow.md) — planned flow through validation, preprocessing, features, training, evaluation and export.

## Historical release notes

- [`releases/v0.1.0-draft.md`](releases/v0.1.0-draft.md) — draft release assessment retained as historical context; it is not a published release.

## Status language

“Implemented” refers to source code present in the repository. “Builds/tests” refers to automated software checks only. Physical wiring, optical calibration, field repeatability, remote credential configuration, and end-to-end device upload require separate validation and should not be inferred from software CI.
