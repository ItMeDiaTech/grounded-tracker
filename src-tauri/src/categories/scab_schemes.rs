use std::collections::HashSet;

use crate::models::{CategoryProgress, ScabSchemeItem};

const TOTAL_SCAB_SCHEMES: u32 = 55;

pub fn extract_scab_schemes(
    collected_themes: &[String],
    achievements: &HashSet<String>,
    progress: &mut CategoryProgress<ScabSchemeItem>,
) {
    let all_obtained = achievements.contains("ObtainSCABs");

    let items: Vec<ScabSchemeItem> = collected_themes
        .iter()
        .map(|name| ScabSchemeItem {
            id: name.to_lowercase(),
            name: format_scheme_name(name),
            collected: true,
        })
        .collect();

    let collected = if all_obtained {
        TOTAL_SCAB_SCHEMES
    } else {
        (items.len() as u32).min(TOTAL_SCAB_SCHEMES)
    };

    progress.items = items;
    progress.total_count = TOTAL_SCAB_SCHEMES;
    progress.collected_count = collected;
}

fn format_scheme_name(raw: &str) -> String {
    match raw {
        "Aerobics" => "Aerobics",
        "Chewgum" => "Table Gum",
        "ChubbsSeries" => "Chubbs Series",
        "CondimentJockey" => "Condiment Jockey",
        "Corporate" => "Corporate Issue",
        "Cyberstrike" => "Cyberstrike",
        "DogLoaf" => "Dog Loaf",
        "FlyingV" => "Flying-V",
        "Frankenline" => "Frankenline",
        "Fright" => "Fright",
        "Frosting" => "Frosting",
        "Garbo" => "Garbo",
        "Ghost" => "Ghostmint",
        "Gingerspice" => "Gingerspice",
        "Girthscape" => "Girthscape",
        "Greystone" => "Greystone",
        "Gunmetal" => "Gunmetal",
        "Hedgeberry" => "Hedgeberry",
        "HighDensity" => "High Density",
        "Holidazzle" => "Holidazzle",
        "HotDogHorror" => "Billy Hog Horror",
        "IceCap" => "Frostbite",
        "Lasercorp" => "Lasercorp",
        "Legend" => "Legend",
        "Marsh" => "Shallows",
        "Metamorphosis" => "Syndrome",
        "Miterider" => "Miterider",
        "Moldorc" => "Moldorc",
        "Murk" => "Muck",
        "Neomauve" => "Neomauve",
        "Panfish" => "Panfish",
        "Porridge" => "Porridge",
        "PunchO" => "Punch-O",
        "QuestModule" => "RPG Module",
        "SCAB1" => "SCA.B v1.02",
        "SCAB2" => "SCA.B v2.11",
        "SCAB3" => "SCA.B v3.09",
        "SCABBugged" => "SCA.B Bugged",
        "SadStab" => "The Sad Stab",
        "Sankematic" => "JavaMatic",
        "Sawdust" => "Sawdust",
        "Sewage" => "Sewage",
        "Shinebright" => "Shinebright",
        "SuperToast" => "Toasted",
        "Superblaster" => "Hyperblaster",
        "Supreme" => "Supreme",
        "Textbook" => "Textbook",
        "Tully" => "Tully",
        "VisualAid" => "Bugjuice",
        "Witch" => "Witchly",
        "YogiSwole" => "Swole",
        _ => raw,
    }
    .to_string()
}
