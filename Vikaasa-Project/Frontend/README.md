# Frontend — Vikāsa Monitoring Dashboard

The frontend is a React 18 + TypeScript single-page application built with Vite, Tailwind CSS, TanStack Query, and Recharts. It presents sensor readings and status information through a shared data-provider interface that supports clearly labelled demo data or the Flask API.

## Main areas

- `src/App.tsx` / `src/main.tsx` — app entry and routing/bootstrap.
- `src/pages/DashboardPages.tsx` — route/page views, including overview, plants, optical sensing, history, alerts, data inspector, and system status.
- `src/components/layout/` — page layout, header, and navigation.
- `src/components/ui/` — reusable cards, badges, status elements, and empty/loading states.
- `src/services/api.ts` — data-provider interface and Flask implementation.
- `src/services/demoData.ts` — simulated sample readings for UI development.
- `src/hooks/useSensorData.ts` — TanStack Query hooks and polling behaviour.
- `src/types/sensor.ts` — frontend reading and response types.
- `src/styles/` — CSS and design tokens.

## Run locally

From `Vikaasa-Project/Frontend/`:

```bash
npm ci
cp .env.example .env.local
# Edit .env.local if connecting to the API
npm run dev
```

For PowerShell, use `Copy-Item .env.example .env.local`. Set:

```env
VITE_API_BASE_URL=http://127.0.0.1:5000
```

This is the Flask server base URL **without `/api`**. The provider adds `/api` internally. If the variable is absent, the app can run in demo mode; simulated readings must not be confused with measurements from hardware.

## Build and lint

```bash
npm run build
npm run lint
npm run preview
```

The CI workflow runs `npm ci`, `npm run build`, and `npm run lint` in this directory.

## Data integrity and display rules

- Raw ADC counts, calibrated reflectance, calibrated NDVI, and the raw NDVI estimate are displayed as distinct values.
- Missing numeric measurements should stay missing/null and display as unavailable rather than being fabricated as zero.
- The CSV inspector validates a user-selected file in the browser and does not merge that file into API/demo sensor readings.
- The API provider polls for current readings; the configured query uses a 10-second refresh interval.
- System-status views should show unverified hardware as unknown rather than inventing a connection result.
- Alerts can be displayed as a page, but the current backend does not generate alert history.

For backend endpoint contracts and semantics, see [`../docs/api.md`](../docs/api.md) and [`../docs/system_architecture.md`](../docs/system_architecture.md). For the project license, see [`../../LICENSE`](../../LICENSE).
