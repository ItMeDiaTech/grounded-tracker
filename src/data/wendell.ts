export interface WendellData {
  id: string;
  name: string;
  subcategory: string;
}

export const WENDELL_TAPES: WendellData[] = [
  // Time At Ominent (7)
  { id: "AudioLogWendellA01", name: "A New Idea", subcategory: "Time At Ominent" },
  { id: "AudioLogWendellA02", name: "Prototype Complete", subcategory: "Time At Ominent" },
  { id: "AudioLogWendellA03", name: "SPAC.R Operational", subcategory: "Time At Ominent" },
  { id: "AudioLogWendellA04", name: "Endless Possibilities", subcategory: "Time At Ominent" },
  { id: "AudioLogWendellA05", name: "Recognition At Last!", subcategory: "Time At Ominent" },
  { id: "AudioLogWendellA06", name: "Yet Another Failure", subcategory: "Time At Ominent" },
  { id: "AudioLogWendellB01", name: "Fired!", subcategory: "Time At Ominent" },

  // SPAC.R Development (9)
  { id: "AudioLogWendellB02", name: "An Auspicious Beginning", subcategory: "SPAC.R Development" },
  { id: "AudioLogWendellB03", name: "Raw Science", subcategory: "SPAC.R Development" },
  { id: "AudioLogWendellB04", name: "Embiggen Test 45", subcategory: "SPAC.R Development" },
  { id: "AudioLogWendellB05", name: "Embiggen Test 50", subcategory: "SPAC.R Development" },
  { id: "NoteRaisinHedge", name: "I'm Tiny!", subcategory: "SPAC.R Development" },
  { id: "AudioLogWendellB06", name: "Embiggening Logistics", subcategory: "SPAC.R Development" },
  { id: "AudioLogWendellB07", name: "SPAC.R Safety Test", subcategory: "SPAC.R Development" },
  { id: "HedgeStorageNote", name: "Lab Modules", subcategory: "SPAC.R Development" },
  { id: "HedgeLabNote", name: "A Little Exhausted", subcategory: "SPAC.R Development" },

  // Hedge (8)
  { id: "AudioLog_Hedge_01", name: "Kaiju Children", subcategory: "Hedge" },
  { id: "AudioLog_Hedge_02", name: "Spider Infestation", subcategory: "Hedge" },
  { id: "AudioLog_Hedge_03", name: "Zip It", subcategory: "Hedge" },
  { id: "AudioLog_Hedge_04", name: "Reboot", subcategory: "Hedge" },
  { id: "AudioLog_Hedge_05", name: "Damage Control", subcategory: "Hedge" },
  { id: "AudioLog_Hedge_06", name: "Re-Reboot", subcategory: "Hedge" },
  { id: "HedgeOverlookNote", name: "Project ZIP.P", subcategory: "Hedge" },
  { id: "HedgeOverlookNote_2", name: "Evacuation", subcategory: "Hedge" },

  // Pond (10)
  { id: "NoteRaisinHaze", name: "Pond Research Note", subcategory: "Pond" },
  { id: "AudioLogWendellPond01", name: "Great Cabbage of Belgium", subcategory: "Pond" },
  { id: "AudioLogWendellPond02", name: "Side Effects", subcategory: "Pond" },
  { id: "PondNote01", name: "HAMS Test: Day 1", subcategory: "Pond" },
  { id: "PondNote02", name: "Fight With Trudy", subcategory: "Pond" },
  { id: "PondNote03", name: "HAMS Test: Day 7", subcategory: "Pond" },
  { id: "PondNote04", name: "HAMS Test: Day 15", subcategory: "Pond" },
  { id: "PondNote05", name: "Muscle Sprouts Test Kitchen", subcategory: "Pond" },
  { id: "PondNote06", name: "HAMS Test: Day 36", subcategory: "Pond" },
  { id: "PondNote07", name: "HAMS Test: Day 91", subcategory: "Pond" },

  // Haze (5)
  { id: "AudioLog_Haze_01", name: "Pipe Problems", subcategory: "Haze" },
  { id: "AudioLog_Haze_02", name: "Trudy Blues", subcategory: "Haze" },
  { id: "AudioLog_Haze_03", name: "Big Brain Time", subcategory: "Haze" },
  { id: "Note_Haze_02", name: "Phase III Results", subcategory: "Haze" },
  { id: "Note_Haze_03", name: "Blazed Earth Protocol", subcategory: "Haze" },

  // Black Anthill (7)
  { id: "AudioLogWendellBAH01", name: "Under Siege", subcategory: "Black Anthill" },
  { id: "AudioLogWendellBAH02", name: "Lab Lockdown", subcategory: "Black Anthill" },
  { id: "BlackAntHill_Lab_Note01", name: "Extra Precautions", subcategory: "Black Anthill" },
  { id: "BlackAntHill_Lab_Note02", name: "MIX.R Trial Summary", subcategory: "Black Anthill" },
  { id: "BlackAntHill_Lab_Note03", name: "Security Briefing", subcategory: "Black Anthill" },
  { id: "BlackAnthill_Lab_Note05", name: "Super Chip Protocol", subcategory: "Black Anthill" },
  { id: "Note_StorageBox_01", name: "MIX.R Relocation Program", subcategory: "Black Anthill" },

  // Undershed (9)
  { id: "AudioLogWendellU01", name: "Going Dark", subcategory: "Undershed" },
  { id: "AudioLogWendellU02", name: "Raw Storage", subcategory: "Undershed" },
  { id: "AudioLogWendellU03", name: "Last Resort", subcategory: "Undershed" },
  { id: "AudioLogWendellU04", name: "Project MANT", subcategory: "Undershed" },
  { id: "Note_Mant_01", name: "Project MANT: Condensed Science", subcategory: "Undershed" },
  { id: "Note_Mant_02", name: "Project MANT: ZIP.P Fiber", subcategory: "Undershed" },
  { id: "Note_Mant_03", name: "Project MANT: Test Drive 1", subcategory: "Undershed" },
  { id: "Note_Mant_04", name: "Muscle Sprout Bonding: Test 18", subcategory: "Undershed" },
  { id: "GrilledScience", name: "Grilled Science", subcategory: "Undershed" },

  // Misc (12)
  { id: "AudioLog_Wendell_SCAB", name: "Goodbye Wendell", subcategory: "Misc" },
  { id: "Oak_Lab_Note1", name: "Imminent Collapse", subcategory: "Misc" },
  { id: "NoteStumpLabEntrance", name: "CCTV Report", subcategory: "Misc" },
  { id: "Note_HiddenKid_Message", name: "Exiled", subcategory: "Misc" },
  { id: "Note_Goodbye_Trudy", name: "Goodbye", subcategory: "Misc" },
  { id: "Note_Wendell_NewGamePlusMachine", name: "Research Notes: Quantum Toast", subcategory: "Misc" },
  { id: "SPACR_GeoTentNote_01", name: "All Of These Eyes", subcategory: "Misc" },
  { id: "OakTree_GeoTentNote01", name: "Local Fauna Report", subcategory: "Misc" },
  { id: "Trash_GeoTentNote_01", name: "New Lab Reconnaissance", subcategory: "Misc" },
  { id: "Sandbox_GeoTentNote_01", name: "Inventory Inspection", subcategory: "Misc" },
  { id: "PicnicTable_GeoTentNote_01", name: "REMINDER: DO NOT OPEN THE BRAWNY BOY BIN!", subcategory: "Misc" },
  { id: "FieldStationMixrNote", name: "Mixing Rigs", subcategory: "Misc" },
];
