import { useState } from "react";
import type React from "react";

type CardVariant = "boss" | "creature" | "compact";

interface ImageCardProps {
  imageSrc: string;
  name: string;
  subtitle?: string;
  status?: string;
  collected: boolean;
  variant?: CardVariant;
}

const IMAGE_HEIGHTS: Record<CardVariant, number> = {
  boss: 140,
  creature: 100,
  compact: 60,
};

export function ImageCard({
  imageSrc,
  name,
  subtitle,
  status,
  collected,
  variant = "creature",
}: ImageCardProps) {
  const [imgError, setImgError] = useState(false);
  const imageHeight = IMAGE_HEIGHTS[variant];

  return (
    <div
      className={collected ? "collected" : "uncollected"}
      style={{
        ...styles.card,
        borderColor: collected ? "var(--accent-green-dim)" : "var(--border-subtle)",
      }}
    >
      <div
        style={{
          ...styles.imageContainer,
          height: imageHeight,
        }}
      >
        {imgError ? (
          <div
            style={{
              ...styles.fallback,
              height: imageHeight,
              width: imageHeight,
            }}
          >
            <span style={styles.fallbackLetter}>
              {name.charAt(0).toUpperCase()}
            </span>
          </div>
        ) : (
          <img
            src={imageSrc}
            alt={name}
            loading="lazy"
            onError={() => setImgError(true)}
            style={{
              ...styles.image,
              height: imageHeight,
            }}
          />
        )}
        {collected && (
          <div style={styles.checkBadge}>&#10003;</div>
        )}
      </div>
      <div style={styles.info}>
        <div
          style={{
            ...styles.name,
            fontSize: variant === "boss" ? "0.95rem" : "0.8rem",
          }}
        >
          {name}
        </div>
        {subtitle && <div style={styles.subtitle}>{subtitle}</div>}
        {status && (
          <div
            style={{
              ...styles.status,
              color: collected ? "var(--accent-green)" : "var(--text-muted)",
            }}
          >
            {status}
          </div>
        )}
      </div>
    </div>
  );
}

const styles: Record<string, React.CSSProperties> = {
  card: {
    display: "flex",
    flexDirection: "column",
    alignItems: "center",
    padding: "var(--space-sm)",
    background: "var(--bg-elevated)",
    border: "1px solid",
    borderRadius: "var(--radius-md)",
    textAlign: "center",
    gap: "var(--space-xs)",
    transition: "all 0.2s ease",
  },
  imageContainer: {
    position: "relative",
    display: "flex",
    alignItems: "center",
    justifyContent: "center",
    overflow: "hidden",
    borderRadius: "var(--radius-sm)",
    width: "100%",
  },
  image: {
    objectFit: "contain",
    maxWidth: "100%",
  },
  fallback: {
    display: "flex",
    alignItems: "center",
    justifyContent: "center",
    borderRadius: "50%",
    background: "var(--bg-hover)",
    border: "2px solid var(--border-subtle)",
  },
  fallbackLetter: {
    fontSize: "1.5rem",
    fontWeight: 700,
    color: "var(--text-muted)",
  },
  checkBadge: {
    position: "absolute",
    top: 4,
    right: 4,
    width: 22,
    height: 22,
    borderRadius: "50%",
    background: "var(--accent-green)",
    color: "#fff",
    display: "flex",
    alignItems: "center",
    justifyContent: "center",
    fontSize: "0.7rem",
    fontWeight: 700,
    boxShadow: "0 1px 4px rgba(0,0,0,0.3)",
  },
  info: {
    display: "flex",
    flexDirection: "column",
    gap: 2,
    width: "100%",
    minWidth: 0,
  },
  name: {
    fontWeight: 600,
    overflow: "hidden",
    textOverflow: "ellipsis",
    whiteSpace: "nowrap",
  },
  subtitle: {
    fontSize: "0.75rem",
    color: "var(--text-muted)",
  },
  status: {
    fontSize: "0.7rem",
    fontWeight: 700,
    letterSpacing: "0.06em",
    marginTop: 2,
  },
};
