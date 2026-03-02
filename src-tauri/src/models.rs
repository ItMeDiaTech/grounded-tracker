use serde::Serialize;

#[derive(Debug, Clone, Serialize, Default)]
#[serde(rename_all = "camelCase")]
pub struct SaveProgress {
    pub save_name: String,
    pub save_path: String,
    pub last_modified: String,
    pub overall_percent: f32,
    pub creature_cards: CategoryProgress<CreatureCardItem>,
    pub landmarks: CategoryProgress<LandmarkItem>,
    pub milk_molars: MilkMolarProgress,
    pub mixr_challenges: CategoryProgress<MixrItem>,
    pub scab_schemes: CategoryProgress<ScabSchemeItem>,
    pub bosses: CategoryProgress<BossItem>,
    pub wendell: CategoryProgress<CollectibleItem>,
    pub ominent: CategoryProgress<CollectibleItem>,
    pub burgl_chips: CategoryProgress<CollectibleItem>,
    pub stuff: CategoryProgress<CollectibleItem>,
}

#[derive(Debug, Clone, Serialize, Default)]
#[serde(rename_all = "camelCase")]
pub struct CategoryProgress<T: Serialize> {
    pub items: Vec<T>,
    pub collected_count: u32,
    pub total_count: u32,
}

#[derive(Debug, Clone, Serialize, Default)]
#[serde(rename_all = "camelCase")]
pub struct CreatureCardItem {
    pub id: String,
    pub name: String,
    pub collected: bool,
}

#[derive(Debug, Clone, Serialize, Default)]
#[serde(rename_all = "camelCase")]
pub struct LandmarkItem {
    pub id: String,
    pub name: String,
    pub zone: String,
    pub discovered: bool,
}

#[derive(Debug, Clone, Serialize, Default)]
#[serde(rename_all = "camelCase")]
pub struct MilkMolarProgress {
    pub regular_collected: u32,
    pub regular_total: u32,
    pub mega_collected: u32,
    pub mega_total: u32,
    pub total_spent: u32,
}

#[derive(Debug, Clone, Serialize, Default)]
#[serde(rename_all = "camelCase")]
pub struct MixrItem {
    pub id: String,
    pub name: String,
    pub completed: bool,
}

#[derive(Debug, Clone, Serialize, Default)]
#[serde(rename_all = "camelCase")]
pub struct ScabSchemeItem {
    pub id: String,
    pub name: String,
    pub collected: bool,
}

#[derive(Debug, Clone, Serialize, Default)]
#[serde(rename_all = "camelCase")]
pub struct BossItem {
    pub id: String,
    pub name: String,
    pub defeated: bool,
}

#[derive(Debug, Clone, Serialize, Default)]
#[serde(rename_all = "camelCase")]
pub struct CollectibleItem {
    pub id: String,
    pub name: String,
    pub subcategory: String,
    pub collected: bool,
}

#[derive(Debug, Clone, Serialize)]
#[serde(rename_all = "camelCase")]
pub struct SaveFolderInfo {
    pub name: String,
    pub path: String,
    pub last_modified: String,
}

#[derive(Debug, Clone, Serialize)]
#[serde(rename_all = "camelCase")]
pub struct WatcherStatus {
    pub watching: bool,
    pub active_save: Option<String>,
    pub last_update: Option<String>,
    pub error: Option<String>,
}
