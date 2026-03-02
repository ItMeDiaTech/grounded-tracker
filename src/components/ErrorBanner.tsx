import type React from "react";

interface ErrorBannerProps {
  message: string;
}

export function ErrorBanner({ message }: ErrorBannerProps) {
  return (
    <div style={styles.banner}>
      <span style={styles.icon}>!</span>
      <span>{message}</span>
    </div>
  );
}

const styles: Record<string, React.CSSProperties> = {
  banner: {
    display: "flex",
    alignItems: "center",
    gap: "var(--space-sm)",
    padding: "var(--space-sm) var(--space-lg)",
    background: "rgba(199, 80, 80, 0.15)",
    borderBottom: "1px solid rgba(199, 80, 80, 0.3)",
    color: "var(--accent-red)",
    fontSize: "0.85rem",
  },
  icon: {
    display: "inline-flex",
    alignItems: "center",
    justifyContent: "center",
    width: 20,
    height: 20,
    borderRadius: "50%",
    background: "var(--accent-red)",
    color: "var(--bg-deep)",
    fontWeight: 700,
    fontSize: "0.75rem",
    flexShrink: 0,
  },
};
