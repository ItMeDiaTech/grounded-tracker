import type React from "react";
import type { CreatureCardItem } from "../types/save-data";
import { ImageCard } from "./ImageCard";

interface CreatureGridProps {
  items: CreatureCardItem[];
}

export function CreatureGrid({ items }: CreatureGridProps) {
  if (items.length === 0) {
    return (
      <div style={styles.empty}>
        Creature card data will appear once save is parsed
      </div>
    );
  }

  return (
    <div style={styles.grid}>
      {items.map((creature) => (
        <ImageCard
          key={creature.id}
          imageSrc={`/images/creatures/${creature.id}.png`}
          name={creature.name}
          collected={creature.collected}
          variant="creature"
        />
      ))}
    </div>
  );
}

const styles: Record<string, React.CSSProperties> = {
  grid: {
    display: "grid",
    gridTemplateColumns: "repeat(auto-fill, minmax(130px, 1fr))",
    gap: "var(--space-sm)",
    padding: "var(--space-md)",
  },
  empty: {
    padding: "var(--space-lg)",
    textAlign: "center",
    color: "var(--text-muted)",
    fontStyle: "italic",
  },
};
