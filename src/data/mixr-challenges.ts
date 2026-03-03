export interface MixrData {
  id: string;
  name: string;
  description: string;
}

// 8 MIX.R challenges (5 regular + 3 Super)
export const MIXR_CHALLENGES: MixrData[] = [
  // Regular MIX.Rs (5)
  { id: "mixr_0", name: "Central Grasslands MIX.R", description: "Regular MIX.R" },
  { id: "mixr_1", name: "Hedge MIX.R", description: "Regular MIX.R" },
  { id: "mixr_2", name: "Haze MIX.R", description: "Regular MIX.R" },
  { id: "mixr_3", name: "Sandbox MIX.R", description: "Regular MIX.R" },
  { id: "mixr_4", name: "Northern Grasslands MIX.R", description: "Regular MIX.R" },

  // Super MIX.Rs (3)
  { id: "mixr_5", name: "Upper Grasslands Super MIX.R", description: "Super MIX.R" },
  { id: "mixr_6", name: "Lawnmower Super MIX.R", description: "Super MIX.R" },
  { id: "mixr_7", name: "Moldorc Super MIX.R", description: "Super MIX.R" },
];
