import type React from "react";

interface HeaderProps {
  overallPercent: number;
  saveName: string;
  lastSync: string | null;
  watching: boolean;
}

export function Header({ overallPercent, saveName, lastSync, watching }: HeaderProps) {
  const percent = Math.round(overallPercent * 10) / 10;
  const isComplete = percent >= 100;

  return (
    <header style={styles.header}>
      <div style={styles.titleRow}>
        <div>
          <h1 style={styles.title}>
            Grounded
            <span style={styles.subtitle}> S Rank Tracker</span>
          </h1>
          <div style={styles.saveName}>{saveName}</div>
        </div>
        <div style={styles.statusArea}>
          <ConnectionDot watching={watching} />
          {lastSync && (
            <span style={styles.lastSync}>Last sync: {lastSync}</span>
          )}
        </div>
      </div>

      <div style={styles.progressContainer}>
        <div style={styles.progressHeader}>
          <span style={styles.progressLabel}>Overall Progress</span>
          <span
            style={{
              ...styles.progressPercent,
              color: isComplete ? "var(--accent-gold)" : "var(--accent-green)",
            }}
          >
            {percent}%
          </span>
        </div>
        <div style={styles.progressTrack}>
          <div
            className="animate-fill"
            style={{
              ...styles.progressFill,
              width: `${Math.min(percent, 100)}%`,
              background: isComplete
                ? "linear-gradient(90deg, var(--accent-gold-dim), var(--accent-gold))"
                : "linear-gradient(90deg, var(--accent-green-dim), var(--accent-green))",
              boxShadow: isComplete
                ? "0 0 12px rgba(212, 168, 67, 0.4)"
                : "0 0 12px var(--accent-green-glow)",
            }}
          />
        </div>
      </div>
    </header>
  );
}

function ConnectionDot({ watching }: { watching: boolean }) {
  return (
    <div style={styles.connectionRow}>
      <div
        style={{
          ...styles.dot,
          backgroundColor: watching ? "var(--accent-green)" : "var(--accent-red)",
          boxShadow: watching
            ? "0 0 6px var(--accent-green-glow)"
            : "none",
        }}
      />
      <span style={styles.connectionLabel}>
        {watching ? "Watching" : "Disconnected"}
      </span>
    </div>
  );
}

const styles: Record<string, React.CSSProperties> = {
  header: {
    background: "var(--bg-base)",
    borderBottom: "1px solid var(--border-subtle)",
    padding: "var(--space-lg)",
  },
  titleRow: {
    display: "flex",
    justifyContent: "space-between",
    alignItems: "flex-start",
    marginBottom: "var(--space-md)",
  },
  title: {
    fontSize: "1.5rem",
    fontWeight: 700,
    color: "var(--text-primary)",
    letterSpacing: "-0.02em",
  },
  subtitle: {
    color: "var(--accent-green)",
    fontWeight: 400,
  },
  saveName: {
    color: "var(--text-muted)",
    fontSize: "0.85rem",
    marginTop: "var(--space-xs)",
  },
  statusArea: {
    display: "flex",
    flexDirection: "column",
    alignItems: "flex-end",
    gap: "var(--space-xs)",
  },
  connectionRow: {
    display: "flex",
    alignItems: "center",
    gap: "var(--space-sm)",
  },
  dot: {
    width: 8,
    height: 8,
    borderRadius: "50%",
  },
  connectionLabel: {
    fontSize: "0.8rem",
    color: "var(--text-secondary)",
  },
  lastSync: {
    fontSize: "0.75rem",
    color: "var(--text-muted)",
  },
  progressContainer: {
    marginTop: "var(--space-sm)",
  },
  progressHeader: {
    display: "flex",
    justifyContent: "space-between",
    marginBottom: "var(--space-xs)",
  },
  progressLabel: {
    fontSize: "0.85rem",
    color: "var(--text-secondary)",
    fontWeight: 500,
  },
  progressPercent: {
    fontSize: "0.85rem",
    fontWeight: 700,
  },
  progressTrack: {
    height: 8,
    background: "var(--bg-elevated)",
    borderRadius: 4,
    overflow: "hidden",
  },
  progressFill: {
    height: "100%",
    borderRadius: 4,
    transition: "width 0.8s ease-out",
  },
};
