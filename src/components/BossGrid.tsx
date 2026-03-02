import type React from "react";
import type { BossItem } from "../types/save-data";
import { ImageCard } from "./ImageCard";

interface BossGridProps {
  items: BossItem[];
}

export function BossGrid({ items }: BossGridProps) {
  if (items.length === 0) {
    return (
      <div style={styles.empty}>
        Boss data will appear once save is parsed
      </div>
    );
  }

  return (
    <div style={styles.grid}>
      {items.map((boss) => (
        <ImageCard
          key={boss.id}
          imageSrc={`/images/bosses/${boss.id}.png`}
          name={boss.name}
          subtitle={boss.location}
          status={boss.defeated ? "DEFEATED" : "UNDEFEATED"}
          collected={boss.defeated}
          variant="boss"
        />
      ))}
    </div>
  );
}

const styles: Record<string, React.CSSProperties> = {
  grid: {
    display: "grid",
    gridTemplateColumns: "repeat(auto-fill, minmax(200px, 1fr))",
    gap: "var(--space-md)",
    padding: "var(--space-md)",
  },
  empty: {
    padding: "var(--space-lg)",
    textAlign: "center",
    color: "var(--text-muted)",
    fontStyle: "italic",
  },
};
