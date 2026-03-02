export interface LandmarkData {
  id: string;
  name: string;
  zone: string;
}

// Landmark POI IDs extracted from actual save files (103 total for S Rank)
// IDs match the /Game/Design/PointsOfInterest/POI_* entries in HostPlayer.csav
export const LANDMARKS: LandmarkData[] = [
  // Grasslands
  { id: "POI_4Leaf_Clover", name: "Four Leaf Clover", zone: "Grasslands" },
  { id: "POI_Ant_Hill_Abandoned", name: "Old Ant Hill", zone: "Grasslands" },
  { id: "POI_Battletoad", name: "Rash", zone: "Grasslands" },
  { id: "POI_Can_Chop", name: "Chop Can", zone: "Grasslands" },
  { id: "POI_Can_Jabby", name: "Jabby Cola", zone: "Grasslands" },
  { id: "POI_Can_Welp", name: "Welp Flavordrink", zone: "Grasslands" },
  { id: "POI_Field_Station_Cave", name: "Larva Tunnels Field Station", zone: "Grasslands" },
  { id: "POI_Field_Station_Grasslands_Chop_Can", name: "Chop Can Field Station", zone: "Grasslands" },
  { id: "POI_Field_Station_Grasslands_Intro", name: "Start Field Station", zone: "Grasslands" },
  { id: "POI_Field_Station_Grasslands_Porch_Branch", name: "Fallen Branch Field Station", zone: "Grasslands" },
  { id: "POI_Field_Station_Grasslands_Rooty_Flood", name: "Flower Bed Field Station", zone: "Grasslands" },
  { id: "POI_Frankenline", name: "Frankenline", zone: "Grasslands" },
  { id: "POI_GardenGnome", name: "Garden Gnome", zone: "Grasslands" },
  { id: "POI_Juicebox_ArmedRaspberry", name: "Armed Raspberry Punch-O", zone: "Grasslands" },
  { id: "POI_Juicebox_LemonCrime", name: "Lemon Crime Punch-O", zone: "Grasslands" },
  { id: "POI_Juicebox_Tropicop", name: "Tropicop Punch-O", zone: "Grasslands" },
  { id: "POI_MarshBaseball", name: "Baseball", zone: "Grasslands" },
  { id: "POI_MilkCarton", name: "Squirts Carton", zone: "Grasslands" },
  { id: "POI_Mint_Container", name: "Candy Dispenser", zone: "Grasslands" },
  { id: "POI_MysteriousLab", name: "Mysterious Lab", zone: "Grasslands" },
  { id: "POI_Oak_Lab", name: "Oak Lab", zone: "Grasslands" },
  { id: "POI_Shrinkpad", name: "Mysterious Machine", zone: "Grasslands" },
  { id: "POI_YokedGirthHead", name: "Yoked Girth Head", zone: "Grasslands" },
  { id: "POI_Defense_Point_RedAnthill", name: "MIX.R Central Grasslands", zone: "Grasslands" },

  // Hedge
  { id: "POI_FlyingDisc", name: "Flingman Flying Disc", zone: "Hedge" },
  { id: "POI_Field_Station_Hedge", name: "Hedge Field Station", zone: "Hedge" },
  { id: "POI_Can_FizzamChuberry", name: "Fizzam Can (Chuberry)", zone: "Hedge" },
  { id: "POI_Hedge_Ascent", name: "Hedge Ascent", zone: "Hedge" },
  { id: "POI_Hedge_Lab", name: "Hedge Lab", zone: "Hedge" },
  { id: "POI_Hedge_Lab_Destroyed_Entrance", name: "Hedge Lab Breach", zone: "Hedge" },
  { id: "POI_Juicebox_Apricop", name: "Apricop Punch-O", zone: "Hedge" },
  { id: "POI_Defense_Point_Hedge", name: "MIX.R Hedge", zone: "Hedge" },

  // Pond
  { id: "POI_BottomPondBackEntry", name: "Depths Mouth", zone: "Pond" },
  { id: "POI_BottomPondLabEntrance", name: "Pond Hatch", zone: "Pond" },
  { id: "POI_BottomPondTrex", name: "Sunken T-Rex", zone: "Pond" },
  { id: "POI_Can_CalvoRootBeer", name: "Calvo Can", zone: "Pond" },
  { id: "POI_Field_Station_UpperPond", name: "Upper Pond Field Station", zone: "Pond" },
  { id: "POI_PondLabDome", name: "Pond Dome", zone: "Pond" },
  { id: "POI_PondLabExit", name: "Pond Lab", zone: "Pond" },
  { id: "POI_PondPagoda", name: "Pond Pagoda Tent", zone: "Pond" },
  { id: "POI_PondPlantPot", name: "Sunken Pot", zone: "Pond" },
  { id: "POI_WeddingRing", name: "Wedding Ring", zone: "Pond" },

  // Haze
  { id: "POI_Defense_Point_Haze", name: "MIX.R Haze", zone: "Haze" },
  { id: "POI_ExposedPipe_Haze", name: "Exposed Pipe (Haze)", zone: "Haze" },
  { id: "POI_Field_Station_Haze", name: "Haze Field Station", zone: "Haze" },
  { id: "POI_Haze_Lab", name: "Haze Lab", zone: "Haze" },
  { id: "POI_Juicebox_PeachFuzz", name: "Peach Fuzz Punch-O", zone: "Haze" },

  // Picnic Table
  { id: "POI_ExposedPipe_WeedArea", name: "Exposed Pipe", zone: "Picnic Table" },
  { id: "POI_Field_Station_PicnicTable", name: "Picnic Table Field Station", zone: "Picnic Table" },
  { id: "POI_MilkMolarBottle", name: "Milk Molar Bottle", zone: "Picnic Table" },
  { id: "POI_PicnicTable_Cooler", name: "Cooler Box", zone: "Picnic Table" },
  { id: "POI_PicnicTable_Shovel", name: "Leaning Shovel", zone: "Picnic Table" },
  { id: "POI_Picnic_Dungeon", name: "Minotaur Maze's End", zone: "Picnic Table" },

  // Trash Heap
  { id: "POI_BAH_TrashEntrance", name: "Black Anthill Descent", zone: "Trash Heap" },
  { id: "POI_BAH_Lab", name: "Black Anthill Lab", zone: "Trash Heap" },
  { id: "POI_Can_CrushedLaBise", name: "Crushed La Bise Can", zone: "Trash Heap" },
  { id: "POI_Field_Station_Trash", name: "Trash Heap Field Station", zone: "Trash Heap" },
  { id: "POI_Trash_CRT", name: "Trashed AARTZ 3000", zone: "Trash Heap" },
  { id: "POI_YokedGirthPackaging", name: "Yoked Girth Blister Pack", zone: "Trash Heap" },

  // Sandbox
  { id: "POI_BAH_SandboxEntrance", name: "Black Anthill Descent (Sandbox)", zone: "Sandbox" },
  { id: "POI_Defense_Point_Sandbox", name: "MIX.R Sandbox", zone: "Sandbox" },
  { id: "POI_Field_Station_Sandbox", name: "Sandbox Field Station", zone: "Sandbox" },
  { id: "POI_Sandbox_LabOutpost", name: "Sandbox Lab Outpost", zone: "Sandbox" },

  // Upper Grasslands
  { id: "POI_AntHill_Jungle", name: "Jungle Anthill", zone: "Upper Grasslands" },
  { id: "POI_AntHill_Lawnmower", name: "Lawnmower Anthill", zone: "Upper Grasslands" },
  { id: "POI_Ashtray", name: "Dirty Ashtray", zone: "Upper Grasslands" },
  { id: "POI_Bat", name: "Bomber Baseball Bat", zone: "Upper Grasslands" },
  { id: "POI_Bike_Thor", name: "Samsquanch Dirt Bike", zone: "Upper Grasslands" },
  { id: "POI_Can_FizzamBlueberry", name: "Fizzam Can (Blueberry)", zone: "Upper Grasslands" },
  { id: "POI_Can_FizzamPamplemousse", name: "Fizzam Can (Pamplemousse)", zone: "Upper Grasslands" },
  { id: "POI_Cap_Gun", name: "Buster Cap Gun", zone: "Upper Grasslands" },
  { id: "POI_Castle_Moldorc_Vista", name: "Castle Moldorc Playset", zone: "Upper Grasslands" },
  { id: "POI_CharcoalHotSpring", name: "Charcoal Hot Springs", zone: "Upper Grasslands" },
  { id: "POI_CootieCatcher", name: "Cootie Catcher", zone: "Upper Grasslands" },
  { id: "POI_CrackedDirt_Glove", name: "Abandoned Gardening Glove", zone: "Upper Grasslands" },
  { id: "POI_Defense_Point_Super_Jungle", name: "Super MIX.R (Jungle)", zone: "Upper Grasslands" },
  { id: "POI_Defense_Point_Super_Lawnmower", name: "Super MIX.R (Lawnmower)", zone: "Upper Grasslands" },
  { id: "POI_Defense_Point_Super_MiniHegde", name: "Super MIX.R (Mini Hedge)", zone: "Upper Grasslands" },
  { id: "POI_Defense_Point_WesternStep", name: "MIX.R Western Step", zone: "Upper Grasslands" },
  { id: "POI_Field_Station_BrickWall", name: "BBQ Field Station", zone: "Upper Grasslands" },
  { id: "POI_Field_Station_EastSteps", name: "East Steps Field Station", zone: "Upper Grasslands" },
  { id: "POI_Field_Station_JavaMatic", name: "JavaMatic Field Station", zone: "Upper Grasslands" },
  { id: "POI_Field_Station_Jungle", name: "Jungle Field Station", zone: "Upper Grasslands" },
  { id: "POI_Field_Station_Junkyard", name: "Junkyard Field Station", zone: "Upper Grasslands" },
  { id: "POI_Field_Station_Oak", name: "Oak Field Station", zone: "Upper Grasslands" },
  { id: "POI_Field_Station_StorageBox", name: "Storage Box Field Station", zone: "Upper Grasslands" },
  { id: "POI_Field_Station_Stump", name: "Stump Field Station", zone: "Upper Grasslands" },
  { id: "POI_Field_Station_Termite_Wood_Pile", name: "Woodpile Field Station", zone: "Upper Grasslands" },
  { id: "POI_JavaMatic", name: "JavaMatic", zone: "Upper Grasslands" },
  { id: "POI_JavamaticOutPost", name: "JavaMatic Storage Facility", zone: "Upper Grasslands" },
  { id: "POI_Lawnmower", name: "Lawnmower", zone: "Upper Grasslands" },
  { id: "POI_SCAB_ShedPorch", name: "Full-Sized SCA.B", zone: "Upper Grasslands" },
  { id: "POI_ShristyToyCar", name: "Christy Doll Car", zone: "Upper Grasslands" },
  { id: "POI_StumpLab", name: "Stump Lab Outpost", zone: "Upper Grasslands" },
  { id: "POI_TazTLarge", name: "Taz.T! Statue", zone: "Upper Grasslands" },
  { id: "POI_Upperyard_Ascent", name: "Upper Yard Ascent", zone: "Upper Grasslands" },
  { id: "POI_Wheelbarrow", name: "Wheelbarrow", zone: "Upper Grasslands" },
  { id: "POI_WoodPileEntrance", name: "Wood Pile Entrance", zone: "Upper Grasslands" },
  { id: "POI_WormHoleContainer", name: "Wormhole Container", zone: "Upper Grasslands" },
  { id: "POI_YogiSwoleHead", name: "Yogi Swole Head", zone: "Upper Grasslands" },

  // Undershed
  { id: "POI_Field_Station_Undershed", name: "Undershed Field Station", zone: "Undershed" },
  { id: "POI_Undershed_Entrance", name: "Undershed Entrance", zone: "Undershed" },
  { id: "POI_Undershed_Lab", name: "Undershed Lab", zone: "Undershed" },
];
