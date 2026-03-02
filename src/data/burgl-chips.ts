export interface BurglChipData {
  id: string;
  name: string;
  subcategory: string;
}

export const BURGL_CHIPS: BurglChipData[] = [
  // Transcription Units (5)
  { id: "AudioLogBurgleA01", name: "Order Number 1", subcategory: "Transcription Units" },
  { id: "AudioLogBurgleA02", name: "Excellent Mustache", subcategory: "Transcription Units" },
  { id: "AudioLogBurgleA03", name: "First Shrinking", subcategory: "Transcription Units" },
  { id: "AudioLogBurgleA04", name: "Web Samples", subcategory: "Transcription Units" },
  { id: "AudioLogBurgleA05", name: "Roby's Song", subcategory: "Transcription Units" },

  // Tech Chips (7)
  { id: "TechChip_AntHill", name: "Red Anthill BURG.L Chip", subcategory: "Tech Chips" },
  { id: "TechChip_PondOutpost", name: "Sunken Outpost BURG.L Chip", subcategory: "Tech Chips" },
  { id: "TechChip_Mint", name: "Sandbox BURG.L Chip", subcategory: "Tech Chips" },
  { id: "TechChip_PicnicTable", name: "Picnic BURG.L Chip", subcategory: "Tech Chips" },
  { id: "TechChip_Woodpile", name: "Woodpile BURG.L Chip", subcategory: "Tech Chips" },
  { id: "TechChip_Stump", name: "Stump BURG.L Chip", subcategory: "Tech Chips" },
  { id: "TechChip_StorageBox", name: "Forgotten BURG.L Chip", subcategory: "Tech Chips" },

  // Super Tech Chips (4)
  { id: "TechChip_Super_Hedge", name: "Hedge Super Chip", subcategory: "Super Tech Chips" },
  { id: "TechChip_Super_Pond", name: "Pond Super Chip", subcategory: "Super Tech Chips" },
  { id: "TechChip_Super_Weed", name: "Haze Super Chip", subcategory: "Super Tech Chips" },
  { id: "TechChip_Super_AntHill_Black", name: "Black Ant Super Chip", subcategory: "Super Tech Chips" },
];
