import type React from "react";
import type { MilkMolarProgress } from "../types/save-data";

interface MilkMolarSectionProps {
  data: MilkMolarProgress;
}

export function MilkMolarSection({ data }: MilkMolarSectionProps) {
  const regularPercent =
    data.regularTotal > 0
      ? (data.regularCollected / data.regularTotal) * 100
      : 0;
  const megaPercent =
    data.megaTotal > 0 ? (data.megaCollected / data.megaTotal) * 100 : 0;

  return (
    <div style={styles.container}>
      <ProgressRow
        label="Regular Milk Molars"
        collected={data.regularCollected}
        total={data.regularTotal}
        percent={regularPercent}
        color="var(--cat-molars)"
      />
      <ProgressRow
        label="Mega Milk Molars"
        collected={data.megaCollected}
        total={data.megaTotal}
        percent={megaPercent}
        color="var(--accent-gold)"
      />
      {data.totalSpent > 0 && (
        <div style={styles.spentRow}>
          <span style={styles.spentLabel}>Total Spent</span>
          <span style={styles.spentValue}>{data.totalSpent}</span>
        </div>
      )}
    </div>
  );
}

function ProgressRow({
  label,
  collected,
  total,
  percent,
  color,
}: {
  label: string;
  collected: number;
  total: number;
  percent: number;
  color: string;
}) {
  return (
    <div style={styles.row}>
      <div style={styles.rowHeader}>
        <span style={styles.rowLabel}>{label}</span>
        <span style={{ ...styles.rowCount, color }}>
          {collected}/{total}
        </span>
      </div>
      <div style={styles.track}>
        <div
          className="animate-fill"
          style={{
            ...styles.fill,
            width: `${Math.min(percent, 100)}%`,
            backgroundColor: color,
          }}
        />
      </div>
    </div>
  );
}

const styles: Record<string, React.CSSProperties> = {
  container: {
    padding: "var(--space-md) var(--space-lg)",
    display: "flex",
    flexDirection: "column",
    gap: "var(--space-md)",
  },
  row: {
    display: "flex",
    flexDirection: "column",
    gap: "var(--space-xs)",
  },
  rowHeader: {
    display: "flex",
    justifyContent: "space-between",
  },
  rowLabel: {
    fontSize: "0.85rem",
    color: "var(--text-secondary)",
  },
  rowCount: {
    fontSize: "0.85rem",
    fontWeight: 700,
    fontVariantNumeric: "tabular-nums",
  },
  track: {
    height: 6,
    background: "var(--bg-elevated)",
    borderRadius: 3,
    overflow: "hidden",
  },
  fill: {
    height: "100%",
    borderRadius: 3,
    transition: "width 0.6s ease-out",
  },
  spentRow: {
    display: "flex",
    justifyContent: "space-between",
    paddingTop: "var(--space-sm)",
    borderTop: "1px solid var(--border-subtle)",
  },
  spentLabel: {
    fontSize: "0.8rem",
    color: "var(--text-muted)",
  },
  spentValue: {
    fontSize: "0.8rem",
    fontWeight: 600,
    color: "var(--text-secondary)",
  },
};
