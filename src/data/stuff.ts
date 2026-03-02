export interface StuffData {
  id: string;
  name: string;
  subcategory: string;
}

export const STUFF_ITEMS: StuffData[] = [
  // Products (2)
  { id: "ScannerBracelet", name: "SCA.B", subcategory: "Products" },
  { id: "SCABManual", name: "SCA.B User Manual", subcategory: "Products" },

  // Statue Recipes (7)
  { id: "RecipeStatueAphid", name: "Aphid Figurine Recipe", subcategory: "Statue Recipes" },
  { id: "RecipeStatueWeevil", name: "Weevil Figurine Recipe", subcategory: "Statue Recipes" },
  { id: "RecipeStatueKoi", name: "Koi Figurine Recipe", subcategory: "Statue Recipes" },
  { id: "RecipeStatueMant", name: "Mant Figurine Recipe", subcategory: "Statue Recipes" },
  { id: "RecipeStatueWendell", name: "Wendell Figurine Recipe", subcategory: "Statue Recipes" },
  { id: "RecipeStatueYokedGirth", name: "Yoked Girth Figurine Recipe", subcategory: "Statue Recipes" },
  { id: "RecipeStatueMoldorc", name: "Moldorc Figurine Recipe", subcategory: "Statue Recipes" },

  // Other Recipes (6)
  { id: "RecipeAntTotem", name: "Ant Totem Recipe", subcategory: "Other Recipes" },
  { id: "RecipeAbominationTotem", name: "Abomination Totem Recipe", subcategory: "Other Recipes" },
  { id: "RecipeSaltMorningStar", name: "Salt Morning Star Recipe", subcategory: "Other Recipes" },
  { id: "RecipeBombArrows", name: "Bomb Arrow Recipe", subcategory: "Other Recipes" },
  { id: "RecipeFridge", name: "Frostbitten Recipe", subcategory: "Other Recipes" },
  { id: "RecipeMantGong", name: "Mant Gong Recipe", subcategory: "Other Recipes" },

  // Keys (6)
  { id: "Key_BlackAntHill", name: "Mossy Key", subcategory: "Keys" },
  { id: "Key_MoatTreasureChest", name: "Melted Moat Key", subcategory: "Keys" },
  { id: "Key_PondTreasureChest", name: "Sticky Key", subcategory: "Keys" },
  { id: "Key_TableTreasureChest", name: "Minotaur Maze Key", subcategory: "Keys" },
  { id: "Key_UpperYardTreasureChest", name: "Upper Yard Treasure Key", subcategory: "Keys" },
  { id: "KeyZiplineAscender", name: "Zipline Ascender", subcategory: "Keys" },

  // Password Scribbles (4)
  { id: "PasswordPieceHedgeLab01", name: "Password Scribble 1", subcategory: "Password Scribbles" },
  { id: "PasswordPieceHedgeLab02", name: "Password Scribble 2", subcategory: "Password Scribbles" },
  { id: "PasswordPieceHedgeLab03", name: "Password Scribble 3", subcategory: "Password Scribbles" },
  { id: "PasswordPieceHedgeLab04", name: "Password Scribble 4", subcategory: "Password Scribbles" },

  // Boss Notes (8)
  { id: "BossNoteBroodmother", name: "Broodmother Research Notes", subcategory: "Boss Notes" },
  { id: "BossNoteMantis", name: "Mantis Research Notes", subcategory: "Boss Notes" },
  { id: "BossNoteAntQueenRedInfect", name: "Infected Red Ant Queen Notes", subcategory: "Boss Notes" },
  { id: "BossNoteAntQueenRedKill", name: "Red Ant Queen Kill Notes", subcategory: "Boss Notes" },
  { id: "BossNoteAntQueenBlackKill", name: "Black Ant Queen Kill Notes", subcategory: "Boss Notes" },
  { id: "BossNoteAntQueenFireKill", name: "Fire Ant Queen Kill Notes", subcategory: "Boss Notes" },
  { id: "BossNoteWasp", name: "Wasp Queen Research Notes", subcategory: "Boss Notes" },
  { id: "BossNoteIBM", name: "Infected Broodmother Notes", subcategory: "Boss Notes" },

  // Super Duper (3)
  { id: "DuperDisc1", name: "Duper Disc 1", subcategory: "Super Duper" },
  { id: "DuperDisc2", name: "Duper Disc 2", subcategory: "Super Duper" },
  { id: "DuperDisc3", name: "Duper Disc 3", subcategory: "Super Duper" },

  // Defense Points (3)
  { id: "DefensePointSuperJungle", name: "Jungle Super Defense Point", subcategory: "Defense Points" },
  { id: "DefensePointSuperUpperMiniHedge", name: "Upper Mini Hedge Super Defense Point", subcategory: "Defense Points" },
  { id: "DefensePointSuperLawnmower", name: "Lawnmower Super Defense Point", subcategory: "Defense Points" },

  // Special (1)
  { id: "GrilledScience", name: "Grilled Science", subcategory: "Special" },
];
