# Backend — Flask REST API

The backend receives sensor readings, validates their schema and measurement relationships, and provides read/write endpoints used by the dashboard. Storage is selected through configuration, allowing local in-memory development and optional Supabase or Firebase integrations.

## Responsibilities

- Create the Flask application and register health, sensor-data, and operational routes.
- Validate the 24-field sensor record with Pydantic (`app/schemas/sensor.py`).
- Keep raw ADC counts separate from calibrated reflectance and NDVI; `raw_ndvi_estimate` is a derived diagnostic field, not a substitute for calibrated NDVI.
- Validate reading identifiers/timestamps and detect duplicate `reading_id` values.
- Use a storage service abstraction with in-memory, Supabase, and Firebase implementations.
- Return explicit system/alert status rather than claiming an unverified ESP32 connection or generated alerts.

## Important files

- `app/__init__.py` — application factory and service wiring.
- `app/config.py` — environment-based configuration.
- `app/routes/` — health, sensor-record, plant/system/alert endpoints.
- `app/schemas/` — request/response contracts.
- `app/services/storage.py` — common storage interface; provider implementations are beside it.
- `database/migrations/001_create_readings.sql` — supplied initial SQL schema/migration for the persistent database setup.
- `tests/` — automated API/storage tests.

## API endpoints

| Method | Endpoint | Purpose |
|---|---|---|
| `GET` | `/api/health` | API and storage health |
| `POST` | `/api/readings` or `/api/sensor-Data` | Submit a reading |
| `GET` | `/api/readings` or `/api/sensor-Data` | Paginated history and filters |
| `GET` | `/api/readings/latest` or `/api/sensor-Data/latest` | Latest reading, optionally filtered by plant |
| `GET` | `/api/plants/summaries` | Per-plant summaries |
| `GET` | `/api/plants/<plant_id>/status` | Plant status/latest reading |
| `GET` | `/api/system/status` | Backend and configured subsystem status |
| `GET` | `/api/alerts` | Alert endpoint; currently returns no generated alerts |

See [`../docs/api.md`](../docs/api.md) for schemas, fields, examples, validation rules, filters, and error responses.

## Local development

Run commands from `Vikaasa-Project/`:

```bash
python -m venv .venv
# Activate the venv, then:
python -m pip install -r requirements.txt
python -m pip install -e ".[dev]"
```

To use a local in-memory store, set `STORAGE_BACKEND=memory` in the environment or copy `.env.example` to `.env` and set that value. Then start the app:

```bash
python -m Backend.app
```

The API normally listens on `http://127.0.0.1:5000`. Run tests from the same project root:

```bash
STORAGE_BACKEND=memory pytest Backend/tests/ -v
```

## Storage options

- **`memory`** — local development/test storage; records disappear when the process exits.
- **`supabase`** — persistent PostgreSQL backend; requires `SUPABASE_URL`, `SUPABASE_SERVICE_ROLE_KEY`, remote schema setup, and responsible credential handling.
- **`firebase`** — Realtime Database adapter; requires project/database settings and a protected service-account credential file or supported application credentials.

Do not place credentials in source control. The `.env.example` file contains variable names and comments only.

## Behaviour and known limits

- Raw ADC inputs are not calibrated reflectance. The API only treats `ndvi` as calibrated NDVI when appropriate reflectance values are provided.
- The default in-memory mode is non-persistent.
- Unit tests mock external provider operations; passing them does not prove a live Supabase/Firebase deployment is configured.
- There is no production authentication or authorization layer. Do not expose the API publicly without adding one.
- `/api/alerts` currently does not generate or persist alerts; Telegram service code should not be described as end-to-end active alerting.
- Physical ESP32 connectivity is not verified by backend tests or CI.
