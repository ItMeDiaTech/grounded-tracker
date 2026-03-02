export interface OminentData {
  id: string;
  name: string;
  subcategory: string;
}

export const OMINENT_ITEMS: OminentData[] = [
  // Recon Journals (9)
  { id: "Note_OminentSpy_001", name: "Recon Journal: Entomologist", subcategory: "Recon Journals" },
  { id: "Note_OminentSpy_002", name: "Recon Journal: Compliance Officer", subcategory: "Recon Journals" },
  { id: "Note_OminentSpy_003", name: "Recon Journal: Intern", subcategory: "Recon Journals" },
  { id: "Note_OminentSpy_004", name: "Recon Journal: Special Forces", subcategory: "Recon Journals" },
  { id: "Note_OminentSpy_005", name: "Recon Journal: Health And Safety Officer", subcategory: "Recon Journals" },
  { id: "Note_OminentSpy_006_a", name: "Recon Journal: Toxicologist (Part 1)", subcategory: "Recon Journals" },
  { id: "Note_OminentSpy_006_b", name: "Recon Journal: Toxicologist (Part 2)", subcategory: "Recon Journals" },
  { id: "Note_OminentSpy_006_c", name: "Recon Journal: Toxicologist (Part 3)", subcategory: "Recon Journals" },
  { id: "Note_OminentSpy_007", name: "Recon Journal: Biomedical Engineer", subcategory: "Recon Journals" },

  // Suspicious Snapshots (6)
  { id: "OminentInfiltrator_Polaroid_Antenna", name: "Suspicious Snapshot: Antenna", subcategory: "Suspicious Snapshots" },
  { id: "OminentInfiltrator_Polaroid_SPACR", name: "Suspicious Snapshot: SPAC.R", subcategory: "Suspicious Snapshots" },
  { id: "OminentInfiltrator_Polaroid_SUPERCHIP", name: "Suspicious Snapshot: Super Chip", subcategory: "Suspicious Snapshots" },
  { id: "OminentInfiltrator_Polaroid_SCAB", name: "Suspicious Snapshot: SCA.B", subcategory: "Suspicious Snapshots" },
  { id: "OminentInfiltrator_Polaroid_JavaMatic", name: "Suspicious Snapshot: Java-Matic", subcategory: "Suspicious Snapshots" },
  { id: "OminentInfiltrator_Polaroid_Muldorc", name: "Suspicious Snapshot: Moldorc", subcategory: "Suspicious Snapshots" },

  // Audio Logs (6)
  { id: "AudioLogOminentA01", name: "Ominent Audio Log 1", subcategory: "Audio Logs" },
  { id: "AudioLogOminentA02", name: "Ominent Audio Log 2", subcategory: "Audio Logs" },
  { id: "AudioLogOminentA03", name: "Ominent Audio Log 3", subcategory: "Audio Logs" },
  { id: "AudioLogOminentA04", name: "Ominent Audio Log 4", subcategory: "Audio Logs" },
  { id: "AudioLogOminentA05", name: "Ominent Audio Log 5", subcategory: "Audio Logs" },
  { id: "AudioLogOminentA06", name: "Ominent Audio Log 6", subcategory: "Audio Logs" },

  // Data (1)
  { id: "Ominent_Disk", name: "Ominent Data Disk", subcategory: "Data" },
];
