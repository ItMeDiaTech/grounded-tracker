export interface StuffData {
  id: string;
  name: string;
  subcategory: string;
}

// 33 Stuff items for 100% Report Card
// IDs from save data, display names from wiki/100% guide
export const STUFF_ITEMS: StuffData[] = [
  // Products (3)
  { id: "ScannerBracelet", name: "SCA.B", subcategory: "Products" },
  { id: "SCABManual", name: "SCA.B User Manual", subcategory: "Products" },
  { id: "KeyZiplineAscender", name: "ZIP.R", subcategory: "Products" },

  // Statues (7)
  { id: "RecipeStatueAphid", name: "Aphid Figurine", subcategory: "Statues" },
  { id: "RecipeStatueWeevil", name: "Weevil Figurine", subcategory: "Statues" },
  { id: "RecipeStatueKoi", name: "Koi Figurine", subcategory: "Statues" },
  { id: "RecipeStatueMant", name: "Mant Figurine", subcategory: "Statues" },
  { id: "RecipeStatueWendell", name: "Wendell Figurine", subcategory: "Statues" },
  { id: "RecipeStatueYokedGirth", name: "Yoked Girth Figurine", subcategory: "Statues" },
  { id: "RecipeStatueMoldorc", name: "Moldorc Figurine", subcategory: "Statues" },

  // Recipe Notes (6)
  { id: "RecipeAntTotem", name: "Ant Totem Recipe", subcategory: "Recipe Notes" },
  { id: "RecipeAbominationTotem", name: "Abomination Totem Recipe", subcategory: "Recipe Notes" },
  { id: "RecipeSaltMorningStar", name: "Water Logged Recipe", subcategory: "Recipe Notes" },
  { id: "RecipeBombArrows", name: "Water Logged Recipe", subcategory: "Recipe Notes" },
  { id: "RecipeFridge", name: "Frostbitten Recipe", subcategory: "Recipe Notes" },
  { id: "RecipeMantGong", name: "Glorious Recipe", subcategory: "Recipe Notes" },

  // Keys (10)
  { id: "PasswordPieceHedgeLab01", name: "Password Scribble: Y-", subcategory: "Keys" },
  { id: "PasswordPieceHedgeLab02", name: "Password Scribble: -UR", subcategory: "Keys" },
  { id: "PasswordPieceHedgeLab03", name: "Password Scribble: T19", subcategory: "Keys" },
  { id: "PasswordPieceHedgeLab04", name: "Password Scribble: 58", subcategory: "Keys" },
  { id: "Key_BlackAntHill", name: "Mossy Key", subcategory: "Keys" },
  { id: "Key_MoatTreasureChest", name: "Melted Moat Key", subcategory: "Keys" },
  { id: "Key_PondTreasureChest", name: "Sticky Key", subcategory: "Keys" },
  { id: "Key_TableTreasureChest", name: "Minotaur Maze Key", subcategory: "Keys" },
  { id: "Key_UpperYardTreasureChest", name: "ASST. Manager Keycard", subcategory: "Keys" },
  { id: "EmbiggeningCocktailFinal", name: "Filled Embiggening Cell", subcategory: "Keys" },

  // Boss Notes (4)
  { id: "BossNoteBroodmother", name: "Broodmother Research Notes", subcategory: "Boss Notes" },
  { id: "BossNoteMantis", name: "Mantis Research Note", subcategory: "Boss Notes" },
  { id: "BossNoteWasp", name: "Memo: O.R.C.", subcategory: "Boss Notes" },
  { id: "BossNoteIBM", name: "Moldy Note", subcategory: "Boss Notes" },

  // Super Duper (3)
  { id: "DuperDisc1", name: "Duper Disc 1", subcategory: "Super Duper" },
  { id: "DuperDisc2", name: "Duper Disc 2", subcategory: "Super Duper" },
  { id: "DuperDisc3", name: "Duper Disc 3", subcategory: "Super Duper" },
];
