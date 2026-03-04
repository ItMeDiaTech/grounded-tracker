export interface SaveProgress {
  saveName: string;
  savePath: string;
  lastModified: string;
  overallPercent: number;
  creatureCards: CategoryProgress<CreatureCardItem>;
  landmarks: CategoryProgress<LandmarkItem>;
  milkMolars: MilkMolarProgress;
  mixrChallenges: CategoryProgress<MixrItem>;
  scabSchemes: CategoryProgress<ScabSchemeItem>;
  bosses: CategoryProgress<BossItem>;
  wendell: CategoryProgress<CollectibleItem>;
  ominent: CategoryProgress<CollectibleItem>;
  burglChips: CategoryProgress<CollectibleItem>;
  stuff: CategoryProgress<CollectibleItem>;
}

export interface CategoryProgress<T> {
  items: T[];
  collectedCount: number;
  totalCount: number;
}

export interface CreatureCardItem {
  id: string;
  name: string;
  collected: boolean;
}

export interface LandmarkItem {
  id: string;
  name: string;
  zone: string;
  discovered: boolean;
}

export interface MilkMolarProgress {
  regularCollected: number;
  regularTotal: number;
  megaCollected: number;
  megaTotal: number;
  totalSpent: number;
}

export interface MixrItem {
  id: string;
  name: string;
  completed: boolean;
}

export interface ScabSchemeItem {
  id: string;
  name: string;
  collected: boolean;
}

export interface BossItem {
  id: string;
  name: string;
  defeated: boolean;
  location?: string;
}

export interface CollectibleItem {
  id: string;
  name: string;
  subcategory: string;
  collected: boolean;
}

export interface ConnectionStatus {
  connected: boolean;
  gameRunning: boolean;
  dllInjected: boolean;
  lastUpdate: string | null;
  error: string | null;
}
