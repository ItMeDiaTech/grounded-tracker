import type React from "react";
import { useState } from "react";

interface CategorySectionProps {
  title: string;
  icon: string;
  collected: number;
  total: number;
  color: string;
  defaultExpanded?: boolean;
  children: React.ReactNode;
}

const ICON_MAP: Record<string, string> = {
  skull: "\u{1F480}",
  bug: "\u{1F41B}",
  map: "\u{1F5FA}",
  tooth: "\u{1F9B7}",
  flask: "\u{1F9EA}",
  palette: "\u{1F3A8}",
  robot: "\u{1F916}",
  brain: "\u{1F9E0}",
  tape: "\u{1F4FC}",
  clipboard: "\u{1F4CB}",
  chip: "\u{1F4BF}",
  package: "\u{1F4E6}",
};

export function CategorySection({
  title,
  icon,
  collected,
  total,
  color,
  defaultExpanded = false,
  children,
}: CategorySectionProps) {
  const [expanded, setExpanded] = useState(defaultExpanded);
  const percent = total > 0 ? (collected / total) * 100 : 0;
  const isComplete = collected >= total && total > 0;

  return (
    <div style={styles.section}>
      <button
        className="category-header"
        onClick={() => setExpanded(!expanded)}
        style={styles.header}
        type="button"
      >
        <div style={styles.headerLeft}>
          <span style={styles.icon}>{ICON_MAP[icon] ?? icon}</span>
          <span style={styles.title}>{title}</span>
          <span
            style={{
              ...styles.count,
              color: isComplete ? "var(--accent-gold)" : color,
            }}
          >
            {collected}/{total}
          </span>
          <span
            style={{
              ...styles.percent,
              color: isComplete ? "var(--accent-gold)" : "var(--text-muted)",
            }}
          >
            ({Math.round(percent)}%)
          </span>
        </div>
        <div style={styles.headerRight}>
          <div style={styles.miniTrack}>
            <div
              style={{
                ...styles.miniFill,
                width: `${Math.min(percent, 100)}%`,
                backgroundColor: isComplete ? "var(--accent-gold)" : color,
              }}
            />
          </div>
          <span
            style={{
              ...styles.chevron,
              transform: expanded ? "rotate(180deg)" : "rotate(0deg)",
            }}
          >
            &#9660;
          </span>
        </div>
      </button>

      <div
        className="accordion-content"
        data-expanded={expanded ? "true" : "false"}
      >
        {children}
      </div>
    </div>
  );
}

const styles: Record<string, React.CSSProperties> = {
  section: {
    background: "var(--bg-base)",
    borderRadius: "var(--radius-md)",
    border: "1px solid var(--border-subtle)",
    overflow: "hidden",
  },
  header: {
    display: "flex",
    justifyContent: "space-between",
    alignItems: "center",
    width: "100%",
    padding: "var(--space-md) var(--space-lg)",
    background: "none",
    border: "none",
    color: "var(--text-primary)",
    cursor: "pointer",
    fontFamily: "inherit",
    fontSize: "inherit",
  },
  headerLeft: {
    display: "flex",
    alignItems: "center",
    gap: "var(--space-sm)",
  },
  icon: {
    fontSize: "1.1rem",
  },
  title: {
    fontWeight: 600,
    fontSize: "0.95rem",
  },
  count: {
    fontWeight: 700,
    fontSize: "0.85rem",
    fontVariantNumeric: "tabular-nums",
  },
  headerRight: {
    display: "flex",
    alignItems: "center",
    gap: "var(--space-md)",
  },
  miniTrack: {
    width: 80,
    height: 4,
    background: "var(--bg-elevated)",
    borderRadius: 2,
    overflow: "hidden",
  },
  miniFill: {
    height: "100%",
    borderRadius: 2,
    transition: "width 0.6s ease-out",
  },
  percent: {
    fontSize: "0.8rem",
    fontWeight: 600,
    fontVariantNumeric: "tabular-nums",
  },
  chevron: {
    fontSize: "0.6rem",
    color: "var(--text-muted)",
    transition: "transform 0.2s ease",
  },
};
