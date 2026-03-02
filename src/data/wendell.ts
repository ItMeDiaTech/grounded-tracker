export interface WendellData {
  id: string;
  name: string;
  subcategory: string;
}

export const WENDELL_TAPES: WendellData[] = [
  // Time At Ominent (5)
  { id: "AudioLogWendellA01", name: "A New Idea", subcategory: "Time At Ominent" },
  { id: "AudioLogWendellA02", name: "Prototype Complete", subcategory: "Time At Ominent" },
  { id: "AudioLogWendellA03", name: "SPAC.R Operational", subcategory: "Time At Ominent" },
  { id: "AudioLogWendellA05", name: "Recognition At Last!", subcategory: "Time At Ominent" },
  { id: "AudioLogWendellA06", name: "Yet Another Failure", subcategory: "Time At Ominent" },

  // SPAC.R Development (7)
  { id: "AudioLogWendellB01", name: "Fired!", subcategory: "SPAC.R Development" },
  { id: "AudioLogWendellB02", name: "An Auspicious Beginning", subcategory: "SPAC.R Development" },
  { id: "AudioLogWendellB03", name: "Raw Science", subcategory: "SPAC.R Development" },
  { id: "AudioLogWendellB04", name: "Embiggen Test 45", subcategory: "SPAC.R Development" },
  { id: "AudioLogWendellB05", name: "Embiggen Test 50", subcategory: "SPAC.R Development" },
  { id: "AudioLogWendellB06", name: "Embiggening Logistics", subcategory: "SPAC.R Development" },
  { id: "AudioLogWendellB07", name: "SPAC.R Safety Test", subcategory: "SPAC.R Development" },

  // Hedge (8)
  { id: "AudioLog_Hedge_01", name: "Kaiju Children", subcategory: "Hedge" },
  { id: "AudioLog_Hedge_02", name: "Spider Infestation", subcategory: "Hedge" },
  { id: "AudioLog_Hedge_03", name: "Project ZIP.P", subcategory: "Hedge" },
  { id: "AudioLog_Hedge_04", name: "Reboot", subcategory: "Hedge" },
  { id: "AudioLog_Hedge_05", name: "Damage Control", subcategory: "Hedge" },
  { id: "AudioLog_Hedge_06", name: "Re-Reboot", subcategory: "Hedge" },
  { id: "HedgeOverlookNote", name: "Hedge Overlook Note", subcategory: "Hedge" },
  { id: "HedgeOverlookNote_2", name: "Hedge Overlook Note 2", subcategory: "Hedge" },

  // Pond (9)
  { id: "AudioLogWendellPond01", name: "Pond Lab Audio Log 1", subcategory: "Pond" },
  { id: "AudioLogWendellPond02", name: "Pond Lab Audio Log 2", subcategory: "Pond" },
  { id: "PondNote01", name: "Pond Research Note 1", subcategory: "Pond" },
  { id: "PondNote02", name: "Pond Research Note 2", subcategory: "Pond" },
  { id: "PondNote03", name: "Pond Research Note 3", subcategory: "Pond" },
  { id: "PondNote04", name: "Pond Research Note 4", subcategory: "Pond" },
  { id: "PondNote05", name: "Pond Research Note 5", subcategory: "Pond" },
  { id: "PondNote06", name: "Pond Research Note 6", subcategory: "Pond" },
  { id: "PondNote07", name: "Pond Research Note 7", subcategory: "Pond" },

  // Haze (5)
  { id: "AudioLog_Haze_01", name: "Pipe Problems", subcategory: "Haze" },
  { id: "AudioLog_Haze_02", name: "Trudy Blues", subcategory: "Haze" },
  { id: "AudioLog_Haze_03", name: "Big Brain Time", subcategory: "Haze" },
  { id: "Note_Haze_02", name: "Haze Lab Note 1", subcategory: "Haze" },
  { id: "Note_Haze_03", name: "Haze Lab Note 2", subcategory: "Haze" },

  // Black Anthill (8)
  { id: "AudioLogWendellBAH01", name: "Under Siege", subcategory: "Black Anthill" },
  { id: "AudioLogWendellBAH02", name: "Lab Lockdown", subcategory: "Black Anthill" },
  { id: "BlackAntHill_Lab_Note01", name: "Black Anthill Lab Note 1", subcategory: "Black Anthill" },
  { id: "BlackAntHill_Lab_Note02", name: "Black Anthill Lab Note 2", subcategory: "Black Anthill" },
  { id: "BlackAntHill_Lab_Note03", name: "Black Anthill Lab Note 3", subcategory: "Black Anthill" },
  { id: "BlackAnthill_Lab_Note05", name: "Black Anthill Lab Note 5", subcategory: "Black Anthill" },
  { id: "Note_StorageBox_01", name: "Storage Box Note", subcategory: "Black Anthill" },
  { id: "FieldStationMixrNote", name: "Field Station MIX.R Note", subcategory: "Black Anthill" },

  // Undershed (8)
  { id: "AudioLogWendellU01", name: "Going Dark", subcategory: "Undershed" },
  { id: "AudioLogWendellU02", name: "Raw Storage", subcategory: "Undershed" },
  { id: "AudioLogWendellU03", name: "Last Resort", subcategory: "Undershed" },
  { id: "AudioLogWendellU04", name: "Project MANT", subcategory: "Undershed" },
  { id: "Note_Mant_01", name: "MANT Lab Note 1", subcategory: "Undershed" },
  { id: "Note_Mant_02", name: "MANT Lab Note 2", subcategory: "Undershed" },
  { id: "Note_Mant_03", name: "MANT Lab Note 3", subcategory: "Undershed" },
  { id: "Note_Mant_04", name: "MANT Lab Note 4", subcategory: "Undershed" },

  // Misc (11)
  { id: "AudioLog_Wendell_SCAB", name: "SCA.B Audio Log", subcategory: "Misc" },
  { id: "Oak_Lab_Note1", name: "Oak Lab Note", subcategory: "Misc" },
  { id: "NoteStumpLabEntrance", name: "Stump Lab Entrance Note", subcategory: "Misc" },
  { id: "Note_HiddenKid_Message", name: "Hidden Message", subcategory: "Misc" },
  { id: "Note_Goodbye_Trudy", name: "Goodbye Trudy", subcategory: "Misc" },
  { id: "Note_Wendell_NewGamePlusMachine", name: "New Game+ Machine Note", subcategory: "Misc" },
  { id: "SPACR_GeoTentNote_01", name: "SPAC.R Geo Tent Note", subcategory: "Misc" },
  { id: "OakTree_GeoTentNote01", name: "Oak Tree Geo Tent Note", subcategory: "Misc" },
  { id: "Trash_GeoTentNote_01", name: "Trash Heap Geo Tent Note", subcategory: "Misc" },
  { id: "Sandbox_GeoTentNote_01", name: "Sandbox Geo Tent Note", subcategory: "Misc" },
  { id: "PicnicTable_GeoTentNote_01", name: "Picnic Table Geo Tent Note", subcategory: "Misc" },
];
