import { useMemo } from "react";
import { useSaveSync } from "./hooks/useSaveSync";
import { Header } from "./components/Header";
import { ErrorBanner } from "./components/ErrorBanner";
import { CategorySection } from "./components/CategorySection";
import { BossGrid } from "./components/BossGrid";
import { CreatureGrid } from "./components/CreatureGrid";
import { LandmarkGrid } from "./components/LandmarkGrid";
import { MilkMolarSection } from "./components/MilkMolarSection";
import { CollectibleGrid } from "./components/CollectibleGrid";
import { LoadingScreen } from "./components/LoadingScreen";
import { BOSSES } from "./data/bosses";
import { CREATURE_CARDS } from "./data/creature-cards";
import { LANDMARKS } from "./data/landmarks";
import { MIXR_CHALLENGES } from "./data/mixr-challenges";
import { SCAB_SCHEMES } from "./data/scab-schemes";
import { WENDELL_TAPES } from "./data/wendell";
import { OMINENT_ITEMS } from "./data/ominent";
import { BURGL_CHIPS } from "./data/burgl-chips";
import { STUFF_ITEMS } from "./data/stuff";
import type {
  BossItem,
  CreatureCardItem,
  LandmarkItem,
  MixrItem,
  ScabSchemeItem,
  CollectibleItem,
} from "./types/save-data";
import "./styles/animations.css";

function App() {
  const { data, status, lastError, lastSync, loading, isStale } = useSaveSync();

  // Merge save data with master data so uncollected items show as faded
  const mergedBosses = useMemo<BossItem[]>(() => {
    const saveItems = data?.bosses.items ?? [];
    const saveById = new Map(saveItems.map((b) => [b.id, b]));
    return BOSSES.map((master) => {
      const save = saveById.get(master.id);
      return {
        id: master.id,
        name: master.name,
        defeated: save?.defeated ?? false,
        location: master.location,
      };
    });
  }, [data?.bosses.items]);

  const mergedCreatureCards = useMemo<CreatureCardItem[]>(() => {
    const saveItems = data?.creatureCards.items ?? [];
    const saveById = new Map(saveItems.map((c) => [c.id, c]));
    return CREATURE_CARDS.map((master) => {
      const save = saveById.get(master.id);
      return {
        id: master.id,
        name: master.name,
        collected: save?.collected ?? false,
      };
    });
  }, [data?.creatureCards.items]);

  const mergedLandmarks = useMemo<LandmarkItem[]>(() => {
    const discoveredIds = new Set(
      (data?.landmarks.items ?? []).map((l) => l.id)
    );
    return LANDMARKS.map((master) => ({
      id: master.id,
      name: master.name,
      zone: master.zone,
      discovered: discoveredIds.has(master.id),
    }));
  }, [data?.landmarks.items]);

  const mergedMixr = useMemo<MixrItem[]>(() => {
    const saveItems = data?.mixrChallenges.items ?? [];
    const saveById = new Map(saveItems.map((m) => [m.id, m]));
    return MIXR_CHALLENGES.map((master) => {
      const save = saveById.get(master.id);
      return {
        id: master.id,
        name: master.name,
        completed: save?.completed ?? false,
      };
    });
  }, [data?.mixrChallenges.items]);

  const mergedScab = useMemo<ScabSchemeItem[]>(() => {
    const collectedIds = new Set(
      (data?.scabSchemes.items ?? []).map((s) => s.id)
    );
    const allObtained =
      data?.scabSchemes.collectedCount === data?.scabSchemes.totalCount &&
      (data?.scabSchemes.totalCount ?? 0) > 0;
    return SCAB_SCHEMES.map((master) => ({
      id: master.id,
      name: master.name,
      location: master.location,
      collected: allObtained || collectedIds.has(master.id),
    }));
  }, [data?.scabSchemes]);

  const mergedWendell = useMemo<CollectibleItem[]>(() => {
    const saveById = new Map(
      (data?.wendell.items ?? []).map((i) => [i.id, i])
    );
    return WENDELL_TAPES.map((master) => ({
      id: master.id,
      name: master.name,
      subcategory: master.subcategory,
      collected: saveById.get(master.id)?.collected ?? false,
    }));
  }, [data?.wendell.items]);

  const mergedOminent = useMemo<CollectibleItem[]>(() => {
    const saveById = new Map(
      (data?.ominent.items ?? []).map((i) => [i.id, i])
    );
    return OMINENT_ITEMS.map((master) => ({
      id: master.id,
      name: master.name,
      subcategory: master.subcategory,
      collected: saveById.get(master.id)?.collected ?? false,
    }));
  }, [data?.ominent.items]);

  const mergedBurglChips = useMemo<CollectibleItem[]>(() => {
    const saveById = new Map(
      (data?.burglChips.items ?? []).map((i) => [i.id, i])
    );
    return BURGL_CHIPS.map((master) => ({
      id: master.id,
      name: master.name,
      subcategory: master.subcategory,
      collected: saveById.get(master.id)?.collected ?? false,
    }));
  }, [data?.burglChips.items]);

  const mergedStuff = useMemo<CollectibleItem[]>(() => {
    const saveById = new Map(
      (data?.stuff.items ?? []).map((i) => [i.id, i])
    );
    return STUFF_ITEMS.map((master) => ({
      id: master.id,
      name: master.name,
      subcategory: master.subcategory,
      collected: saveById.get(master.id)?.collected ?? false,
    }));
  }, [data?.stuff.items]);

  if (loading && !data) {
    return <LoadingScreen />;
  }

  return (
    <div style={styles.app}>
      <Header
        overallPercent={data?.overallPercent ?? 0}
        saveName={data?.saveName ?? "Waiting for connection"}
        lastSync={lastSync}
        gameRunning={status?.gameRunning ?? false}
        connected={status?.connected ?? false}
      />

      {lastError && <ErrorBanner message={lastError} />}

      {isStale && data && (
        <div style={styles.staleBanner}>
          Data may be outdated — waiting to reconnect...
        </div>
      )}

      <main style={styles.main}>
        {data && (
          <div style={{ ...styles.categories, opacity: isStale ? 0.7 : 1 }}>
            <CategorySection
              title="Bosses"
              icon="skull"
              collected={data.bosses.collectedCount}
              total={data.bosses.totalCount}
              color="var(--cat-bosses)"
              defaultExpanded
            >
              <BossGrid items={mergedBosses} />
            </CategorySection>

            <CategorySection
              title="Creature Cards"
              icon="bug"
              collected={data.creatureCards.collectedCount}
              total={data.creatureCards.totalCount}
              color="var(--cat-creatures)"
            >
              <CreatureGrid items={mergedCreatureCards} />
            </CategorySection>

            <CategorySection
              title="Landmarks"
              icon="map"
              collected={data.landmarks.collectedCount}
              total={data.landmarks.totalCount}
              color="var(--cat-landmarks)"
            >
              <LandmarkGrid items={mergedLandmarks} />
            </CategorySection>

            <CategorySection
              title="Milk Molars"
              icon="tooth"
              collected={
                data.milkMolars.regularCollected + data.milkMolars.megaCollected
              }
              total={
                data.milkMolars.regularTotal + data.milkMolars.megaTotal
              }
              color="var(--cat-molars)"
            >
              <MilkMolarSection data={data.milkMolars} />
            </CategorySection>

            <CategorySection
              title="MIX.R Challenges"
              icon="flask"
              collected={data.mixrChallenges.collectedCount}
              total={data.mixrChallenges.totalCount}
              color="var(--cat-mixr)"
            >
              <ItemGrid
                items={mergedMixr}
                labelKey="name"
                collectedKey="completed"
              />
            </CategorySection>

            <CategorySection
              title="SCA.B Schemes"
              icon="palette"
              collected={data.scabSchemes.collectedCount}
              total={data.scabSchemes.totalCount}
              color="var(--cat-scab)"
            >
              <ItemGrid
                items={mergedScab}
                labelKey="name"
                collectedKey="collected"
                subtitleKey="location"
              />
            </CategorySection>

            <CategorySection
              title="Wendell Tapes"
              icon="tape"
              collected={data.wendell.collectedCount}
              total={data.wendell.totalCount}
              color="var(--cat-wendell)"
            >
              <CollectibleGrid items={mergedWendell} />
            </CategorySection>

            <CategorySection
              title="Ominent"
              icon="clipboard"
              collected={data.ominent.collectedCount}
              total={data.ominent.totalCount}
              color="var(--cat-ominent)"
            >
              <CollectibleGrid items={mergedOminent} />
            </CategorySection>

            <CategorySection
              title="BURG.L Chips"
              icon="chip"
              collected={data.burglChips.collectedCount}
              total={data.burglChips.totalCount}
              color="var(--cat-burgl-chips)"
            >
              <CollectibleGrid items={mergedBurglChips} />
            </CategorySection>

            <CategorySection
              title="Stuff"
              icon="package"
              collected={data.stuff.collectedCount}
              total={data.stuff.totalCount}
              color="var(--cat-stuff)"
            >
              <CollectibleGrid items={mergedStuff} />
            </CategorySection>
          </div>
        )}
      </main>
    </div>
  );
}

// Generic grid for items with a label and collected boolean
function ItemGrid({
  items,
  labelKey,
  collectedKey,
  subtitleKey,
}: {
  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  items: any[];
  labelKey: string;
  collectedKey: string;
  subtitleKey?: string;
}) {
  if (items.length === 0) {
    return (
      <div style={styles.emptyState}>
        No individual item data available yet
      </div>
    );
  }

  return (
    <div style={styles.itemGrid}>
      {items.map((item, i) => {
        const label = String(item[labelKey] ?? `Item ${i + 1}`);
        const subtitle = subtitleKey ? String(item[subtitleKey] ?? "") : "";
        const collected = Boolean(item[collectedKey]);
        return (
          <div
            key={String(item.id ?? i)}
            className={collected ? "collected" : "uncollected"}
            style={{
              ...styles.itemCard,
              borderColor: collected
                ? "var(--accent-green-dim)"
                : "var(--border-subtle)",
            }}
          >
            <div style={{ display: "flex", flexDirection: "column", flex: 1, overflow: "hidden" }}>
              <span style={styles.itemLabel}>{label}</span>
              {subtitle && (
                <span style={styles.itemSubtitle}>{subtitle}</span>
              )}
            </div>
            {collected && <span style={styles.checkmark}>&#10003;</span>}
          </div>
        );
      })}
    </div>
  );
}

const styles: Record<string, React.CSSProperties> = {
  app: {
    minHeight: "100vh",
    display: "flex",
    flexDirection: "column",
    background: "var(--bg-deep)",
  },
  main: {
    flex: 1,
    padding: "var(--space-lg)",
    maxWidth: 1200,
    margin: "0 auto",
    width: "100%",
  },
  categories: {
    display: "flex",
    flexDirection: "column",
    gap: "var(--space-sm)",
  },
  itemGrid: {
    display: "grid",
    gridTemplateColumns: "repeat(auto-fill, minmax(180px, 1fr))",
    gap: "var(--space-sm)",
    padding: "var(--space-md)",
  },
  itemCard: {
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
  itemLabel: {
    overflow: "hidden",
    textOverflow: "ellipsis",
    whiteSpace: "nowrap",
  },
  itemSubtitle: {
    fontSize: "0.75rem",
    color: "var(--text-muted)",
    overflow: "hidden",
    textOverflow: "ellipsis",
    whiteSpace: "nowrap",
  },
  checkmark: {
    color: "var(--accent-green)",
    fontWeight: "bold",
    marginLeft: "var(--space-sm)",
    flexShrink: 0,
  },
  staleBanner: {
    background: "rgba(217, 164, 6, 0.15)",
    color: "#d9a406",
    padding: "var(--space-sm) var(--space-lg)",
    textAlign: "center" as const,
    fontSize: "0.9rem",
    borderBottom: "1px solid rgba(217, 164, 6, 0.3)",
  },
  emptyState: {
    padding: "var(--space-lg)",
    textAlign: "center",
    color: "var(--text-muted)",
    fontStyle: "italic",
  },
};

export default App;
