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
  { id: "Note_OminentSpy_006_a", name: "A Sticky Situation", subcategory: "Recon Journals" },
  { id: "Note_OminentSpy_006_b", name: "Loyal Subjects", subcategory: "Recon Journals" },
  { id: "Note_OminentSpy_006_c", name: "Recon Journal: Toxicologist", subcategory: "Recon Journals" },
  { id: "Note_OminentSpy_007", name: "Recon Journal: Biomedical Engineer", subcategory: "Recon Journals" },

  // Suspicious Snapshots (6)
  { id: "OminentInfiltrator_Polaroid_Antenna", name: "Suspicious Snapshot: Antenna", subcategory: "Suspicious Snapshots" },
  { id: "OminentInfiltrator_Polaroid_SPACR", name: "Suspicious Snapshot: SPAC.R", subcategory: "Suspicious Snapshots" },
  { id: "OminentInfiltrator_Polaroid_SUPERCHIP", name: "Suspicious Snapshot: Super Chip", subcategory: "Suspicious Snapshots" },
  { id: "OminentInfiltrator_Polaroid_SCAB", name: "Suspicious Snapshot: SCA.B", subcategory: "Suspicious Snapshots" },
  { id: "OminentInfiltrator_Polaroid_JavaMatic", name: "Suspicious Snapshot: Java-Matic", subcategory: "Suspicious Snapshots" },
  { id: "OminentInfiltrator_Polaroid_Muldorc", name: "Suspicious Snapshot: Moldorc", subcategory: "Suspicious Snapshots" },

  // Data (1)
  { id: "Ominent_Disk", name: "Ominent Data Disk", subcategory: "Data" },
];
