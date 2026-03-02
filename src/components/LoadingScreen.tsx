import type React from "react";

export function LoadingScreen() {
  return (
    <div style={styles.container}>
      <div className="spinner" />
      <div style={styles.text}>Loading save data...</div>
      <div style={styles.subtext}>
        Watching for Grounded save files
      </div>
    </div>
  );
}

const styles: Record<string, React.CSSProperties> = {
  container: {
    display: "flex",
    flexDirection: "column",
    alignItems: "center",
    justifyContent: "center",
    minHeight: "100vh",
    gap: "var(--space-md)",
  },
  text: {
    fontSize: "1rem",
    color: "var(--text-primary)",
    fontWeight: 500,
  },
  subtext: {
    fontSize: "0.8rem",
    color: "var(--text-muted)",
  },
};
