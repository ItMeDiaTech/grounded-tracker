import type React from "react";
import type { LandmarkItem } from "../types/save-data";

interface LandmarkGridProps {
  items: LandmarkItem[];
}

const ZONE_ICONS: Record<string, string> = {
  Grasslands: "\u{1F33F}",
  Hedge: "\u{1F333}",
  Pond: "\u{1F41F}",
  Haze: "\u{1F32B}\uFE0F",
  "Picnic Table": "\u{1F9FA}",
  "Trash Heap": "\u{1F5D1}\uFE0F",
  Sandbox: "\u{1F3D6}\uFE0F",
  "Upper Grasslands": "\u{2600}\uFE0F",
  Undershed: "\u{1F3E0}",
};

export function LandmarkGrid({ items }: LandmarkGridProps) {
  if (items.length === 0) {
    return (
      <div style={styles.empty}>
        Landmark data will appear once save is parsed
      </div>
    );
  }

  // Group by zone, preserving order of first appearance
  const zoneOrder: string[] = [];
  const zoneMap = new Map<string, LandmarkItem[]>();
  for (const item of items) {
    const zone = item.zone || "Various";
    if (!zoneMap.has(zone)) {
      zoneOrder.push(zone);
      zoneMap.set(zone, []);
    }
    zoneMap.get(zone)!.push(item);
  }

  return (
    <div style={styles.container}>
      {zoneOrder.map((zone) => {
        const zoneItems = zoneMap.get(zone)!;
        const discoveredCount = zoneItems.filter((l) => l.discovered).length;
        const icon = ZONE_ICONS[zone] ?? "\u{1F4CD}";

        return (
          <div key={zone} style={styles.zoneGroup}>
            <div style={styles.zoneHeader}>
              <span>{icon}</span>
              <span style={styles.zoneName}>{zone}</span>
              <span
                style={{
                  ...styles.zoneCount,
                  color:
                    discoveredCount === zoneItems.length
                      ? "var(--accent-gold)"
                      : "var(--text-muted)",
                }}
              >
                {discoveredCount}/{zoneItems.length}
              </span>
            </div>
            <div style={styles.grid}>
              {zoneItems.map((landmark) => (
                <div
                  key={landmark.id}
                  className={landmark.discovered ? "collected" : "uncollected"}
                  style={{
                    ...styles.card,
                    borderColor: landmark.discovered
                      ? "var(--accent-green-dim)"
                      : "var(--border-subtle)",
                  }}
                >
                  <span style={styles.pin}>{"\u{1F4CD}"}</span>
                  <span style={styles.landmarkName}>{landmark.name}</span>
                  {landmark.discovered && (
                    <span style={styles.check}>&#10003;</span>
                  )}
                </div>
              ))}
            </div>
          </div>
        );
      })}
    </div>
  );
}

const styles: Record<string, React.CSSProperties> = {
  container: {
    display: "flex",
    flexDirection: "column",
    gap: "var(--space-md)",
    padding: "var(--space-md)",
  },
  zoneGroup: {
    display: "flex",
    flexDirection: "column",
    gap: "var(--space-sm)",
  },
  zoneHeader: {
    display: "flex",
    alignItems: "center",
    gap: "var(--space-sm)",
    paddingBottom: "var(--space-xs)",
    borderBottom: "1px solid var(--border-subtle)",
  },
  zoneName: {
    fontWeight: 600,
    fontSize: "0.9rem",
  },
  zoneCount: {
    fontSize: "0.8rem",
    fontWeight: 600,
    fontVariantNumeric: "tabular-nums",
  },
  grid: {
    display: "grid",
    gridTemplateColumns: "repeat(auto-fill, minmax(200px, 1fr))",
    gap: "var(--space-sm)",
  },
  card: {
    display: "flex",
    alignItems: "center",
    gap: "var(--space-sm)",
    padding: "var(--space-sm) var(--space-md)",
    background: "var(--bg-elevated)",
    border: "1px solid",
    borderRadius: "var(--radius-sm)",
    fontSize: "0.85rem",
    transition: "all 0.2s ease",
  },
  pin: {
    fontSize: "0.9rem",
    flexShrink: 0,
  },
  landmarkName: {
    flex: 1,
    overflow: "hidden",
    textOverflow: "ellipsis",
    whiteSpace: "nowrap",
  },
  check: {
    color: "var(--accent-green)",
    fontWeight: "bold",
    flexShrink: 0,
  },
  empty: {
    padding: "var(--space-lg)",
    textAlign: "center",
    color: "var(--text-muted)",
    fontStyle: "italic",
  },
};
