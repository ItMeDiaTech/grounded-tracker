export interface ScabSchemeData {
  id: string;
  name: string;
  location: string;
}

// All 55 SCAB color schemes (4 default + 51 collectible)
// IDs match GlobalColorTheme_ suffixes from save data (lowercased)
export const SCAB_SCHEMES: ScabSchemeData[] = [
  // Default (4) — unlocked from game start
  { id: "default", name: "SCA.B Classic", location: "Default" },
  { id: "highcontrast", name: "High Contrast", location: "Default" },
  { id: "lcd", name: "SCA.B LCD", location: "Default" },
  { id: "night", name: "SCA.B Night", location: "Default" },

  // Collectible (51) — found throughout the backyard
  { id: "aerobics", name: "Aerobics", location: "Oak Hill, Garden Lamp" },
  { id: "hotdoghorror", name: "Billy Hog Horror", location: "Koi Pond, Calvo Can" },
  { id: "visualaid", name: "Bugjuice", location: "Hedge, Ominent Outpost" },
  { id: "chubbsseries", name: "Chubbs Series", location: "Flooded Zone, Clover Cave" },
  { id: "condimentjockey", name: "Condiment Jockey", location: "Brawny Boy Bin" },
  { id: "corporate", name: "Corporate Issue", location: "Black Anthill" },
  { id: "cyberstrike", name: "Cyberstrike", location: "Flooded Zone, Power Outlet" },
  { id: "dogloaf", name: "Dog Loaf", location: "Picnic Table, Plastic Knife" },
  { id: "flyingv", name: "Flying-V", location: "Rake Rock Point" },
  { id: "frankenline", name: "Frankenline", location: "Frankenline, Flower Bed" },
  { id: "fright", name: "Fright", location: "Hedge Lab Entrance" },
  { id: "icecap", name: "Frostbite", location: "Pond Lab" },
  { id: "frosting", name: "Frosting", location: "Brawny Boy Bin, Wasp Nest" },
  { id: "garbo", name: "Garbo", location: "Trash Heap, Cooler" },
  { id: "ghost", name: "Ghostmint", location: "Hedge Lab" },
  { id: "gingerspice", name: "Gingerspice", location: "Pond Lab" },
  { id: "girthscape", name: "Girthscape", location: "Yoked Girth Head Cave" },
  { id: "greystone", name: "Greystone", location: "IceCaps Mint Container" },
  { id: "gunmetal", name: "Gunmetal", location: "Spade Gulch Cave" },
  { id: "hedgeberry", name: "Hedgeberry", location: "Hedge, Bird Bath" },
  { id: "highdensity", name: "High Density", location: "Shed Surroundings, Termite Cave" },
  { id: "holidazzle", name: "Holidazzle", location: "Koi Pond, Ominent Outpost" },
  { id: "superblaster", name: "Hyperblaster", location: "Flooded Zone, East Laser" },
  { id: "sankematic", name: "JavaMatic", location: "Upper Yard, JavaMatic" },
  { id: "lasercorp", name: "Lasercorp", location: "Dry Grasslands, Paint Can" },
  { id: "legend", name: "Legend", location: "Hedge, Fizzam Can" },
  { id: "miterider", name: "Miterider", location: "Southwest Laser Cave" },
  { id: "moldorc", name: "Moldorc", location: "Castle Moldorc Playset" },
  { id: "murk", name: "Muck", location: "Pond Lab" },
  { id: "neomauve", name: "Neomauve", location: "Westerly Ant Hill" },
  { id: "panfish", name: "Panfish", location: "Koi Pond, Oak Hill" },
  { id: "porridge", name: "Porridge", location: "Moldorc Highlands" },
  { id: "puncho", name: "Punch-O", location: "Tropicop Juice Box" },
  { id: "questmodule", name: "RPG Module", location: "Minotaur Maze's End" },
  { id: "sawdust", name: "Sawdust", location: "Termite Den" },
  { id: "scab1", name: "SCA.B v1.02", location: "Under House Deck" },
  { id: "scab2", name: "SCA.B v2.11", location: "Hedge Lab, Observation Pod" },
  { id: "scab3", name: "SCA.B v3.09", location: "Stepping Stone Summit" },
  { id: "scabbugged", name: "SCA.B Bugged", location: "Old Ant Hill" },
  { id: "sewage", name: "Sewage", location: "Haze, Exposed Pipe" },
  { id: "marsh", name: "Shallows", location: "Pond Lab, Dome Room" },
  { id: "shinebright", name: "Shinebright", location: "Oak Lab, Pond Cave" },
  { id: "supreme", name: "Supreme", location: "Hedge, Flingman Flying Disc" },
  { id: "metamorphosis", name: "Syndrome", location: "Rake Rock, Spider Caves" },
  { id: "yogiswole", name: "Swole", location: "Upper Yard, Yogi Swole Head" },
  { id: "chewgum", name: "Table Gum", location: "Upper Yard, Fallen Ladder" },
  { id: "textbook", name: "Textbook", location: "Flooded Zone, Sprinklers" },
  { id: "sadstab", name: "The Sad Stab", location: "Eastern Yard, Cassette Tape" },
  { id: "supertoast", name: "Toasted", location: "REMIX.R Lab, Flower Bed" },
  { id: "tully", name: "Tully", location: "Undershed Lab" },
  { id: "witch", name: "Witchly", location: "Hedge, Isolated Branch" },
];
