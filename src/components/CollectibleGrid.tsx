import type React from "react";
import type { CollectibleItem } from "../types/save-data";

interface CollectibleGridProps {
  items: CollectibleItem[];
}

export function CollectibleGrid({ items }: CollectibleGridProps) {
  if (items.length === 0) {
    return (
      <div style={styles.empty}>
        No individual item data available yet
      </div>
    );
  }

  // Group by subcategory, preserving order of first appearance
  const subOrder: string[] = [];
  const subMap = new Map<string, CollectibleItem[]>();
  for (const item of items) {
    const sub = item.subcategory || "Other";
    if (!subMap.has(sub)) {
      subOrder.push(sub);
      subMap.set(sub, []);
    }
    subMap.get(sub)!.push(item);
  }

  return (
    <div style={styles.container}>
      {subOrder.map((sub) => {
        const subItems = subMap.get(sub)!;
        const collectedCount = subItems.filter((i) => i.collected).length;

        return (
          <div key={sub} style={styles.subGroup}>
            <div style={styles.subHeader}>
              <span style={styles.subName}>{sub}</span>
              <span
                style={{
                  ...styles.subCount,
                  color:
                    collectedCount === subItems.length
                      ? "var(--accent-gold)"
                      : "var(--text-muted)",
                }}
              >
                {collectedCount}/{subItems.length}
              </span>
            </div>
            <div style={styles.grid}>
              {subItems.map((item) => (
                <div
                  key={item.id}
                  className={item.collected ? "collected" : "uncollected"}
                  style={{
                    ...styles.card,
                    borderColor: item.collected
                      ? "var(--accent-green-dim)"
                      : "var(--border-subtle)",
                  }}
                >
                  <span style={styles.itemName}>{item.name}</span>
                  {item.collected && (
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
  subGroup: {
    display: "flex",
    flexDirection: "column",
    gap: "var(--space-sm)",
  },
  subHeader: {
    display: "flex",
    alignItems: "center",
    gap: "var(--space-sm)",
    paddingBottom: "var(--space-xs)",
    borderBottom: "1px solid var(--border-subtle)",
  },
  subName: {
    fontWeight: 600,
    fontSize: "0.9rem",
  },
  subCount: {
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
    justifyContent: "space-between",
    padding: "var(--space-sm) var(--space-md)",
    background: "var(--bg-elevated)",
    border: "1px solid",
    borderRadius: "var(--radius-sm)",
    fontSize: "0.85rem",
    transition: "all 0.2s ease",
  },
  itemName: {
    flex: 1,
    overflow: "hidden",
    textOverflow: "ellipsis",
    whiteSpace: "nowrap",
  },
  check: {
    color: "var(--accent-green)",
    fontWeight: "bold",
    flexShrink: 0,
    marginLeft: "var(--space-sm)",
  },
  empty: {
    padding: "var(--space-lg)",
    textAlign: "center",
    color: "var(--text-muted)",
    fontStyle: "italic",
  },
};
