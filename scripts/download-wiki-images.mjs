#!/usr/bin/env node
/**
 * Downloads boss and creature images from the Grounded Wiki (Fandom).
 * Uses native fetch (Node 18+), no npm dependencies.
 *
 * Usage: node scripts/download-wiki-images.mjs
 *
 * Idempotent — skips files that already exist.
 */

import { writeFile, access, mkdir } from "node:fs/promises";
import { join, dirname } from "node:path";
import { fileURLToPath } from "node:url";

const __dirname = dirname(fileURLToPath(import.meta.url));
const ROOT = join(__dirname, "..");
const BOSS_DIR = join(ROOT, "public", "images", "bosses");
const CREATURE_DIR = join(ROOT, "public", "images", "creatures");

const FANDOM_API = "https://grounded.fandom.com/api.php";
const WIKI_GG_BASE = "https://grounded.wiki.gg/images";
const THUMB_SIZE = 256;

// Direct image URLs from wiki.gg for creatures missing from Fandom
const WIKI_GG_FALLBACKS = {
  broodmother: `${WIKI_GG_BASE}/Hedge_Broodmother.png`,
  orb_weaver_jr: `${WIKI_GG_BASE}/Orb_Weaver_Jr..png`,
  orchid_mantis: `${WIKI_GG_BASE}/Orchid_Mantis.png`,
};

// Boss ID -> Wiki page title
const BOSSES = {
  killbroodmother: "Hedge_Broodmother",
  killinfectedbroodmother: "Infected_Broodmother",
  killwaspqueen: "Wasp_Queen",
  killdirector: "Assistant_Manager",
  killmantis: "Mantis",
};

// Creature card ID -> Wiki page title (68 entries)
const CREATURES = {
  // Neutral (8)
  ant_red_worker: "Red_Worker_Ant",
  ant_black_worker: "Black_Worker_Ant",
  beetle_ladybug: "Ladybug",
  bee: "Bee",
  firefly: "Firefly",
  sickly_roly_poly: "Sickly_Roly_Poly",
  beetle_roly_poly: "Roly_Poly",
  moth: "Moth",

  // Angry (48)
  ant_red_soldier: "Red_Soldier_Ant",
  lawn_mite: "Lawn_Mite",
  larva: "Larva",
  ruzt: "RUZ.T",
  water_flea: "Water_Flea",
  orb_weaver_jr: "Orb_Weaver_Jr.",
  spiderling: "Spiderling",
  ant_black_soldier: "Black_Soldier_Ant",
  tayzt: "TAYZ.T",
  arcr: "ARC.R",
  mosquito: "Mosquito",
  infected_weevil: "Infected_Weevil",
  infected_mite: "Infected_Mite",
  infected_ladybug: "Infected_Ladybug",
  infected_larva: "Infected_Larva",
  infected_gnat: "Infected_Gnat",
  beetle_stinkbug: "Stinkbug",
  orb_weaver: "Orb_Weaver",
  diving_bell_spider: "Diving_Bell_Spider",
  wolf_spider: "Wolf_Spider",
  beetle_bombardier: "Bombardier_Beetle",
  ant_fire_worker: "Fire_Worker_Ant",
  ant_fire_soldier: "Fire_Soldier_Ant",
  dust_mite: "Dust_Mite",
  ladybird_larva: "Ladybird_Larva",
  ant_lion: "Antlion",
  termite_worker: "Termite_Worker",
  termite_soldier: "Termite_Soldier",
  termite_king: "Termite_King",
  black_ox_beetle: "Black_Ox_Beetle",
  ladybird: "Ladybird",
  green_shield_bug: "Green_Shield_Bug",
  infected_wolf_spider: "Infected_Wolf_Spider",
  black_widow: "Black_Widow",
  spiny_water_flea: "Spiny_Water_Flea",
  tiger_mosquito: "Tiger_Mosquito",
  tick: "Tick",
  black_widowling: "Black_Widowling",
  wasp: "Wasp",
  wasp_drone: "Wasp_Drone",
  assistant_manager: "Assistant_Manager",
  broodmother: "Hedge_Broodmother",
  mant: "Mant",
  orchid_mantis: "Orchid_Mantis",
  director_schmector: "Director_Schmector",
  wasp_queen: "Wasp_Queen",
  infected_broodmother: "Infected_Broodmother",
  koi_fish: "Koi_Fish",

  // Harmless (12)
  aphid: "Aphid",
  weevil: "Weevil",
  grub: "Grub",
  gnat: "Gnat",
  meaty_gnat: "Meaty_Gnat",
  tadpole: "Tadpole",
  water_boatman: "Water_Boatman",
  scarab: "Scarab",
  red_ant_queen: "Red_Ant_Queen",
  black_ant_queen: "Black_Ant_Queen",
  fire_ant_queen: "Fire_Ant_Queen",
  crow: "Crow",
};

async function fileExists(path) {
  try {
    await access(path);
    return true;
  } catch {
    return false;
  }
}

/**
 * Query the Fandom MediaWiki API for page images in batches.
 * Returns a Map of wikiTitle -> imageUrl.
 */
async function fetchPageImages(titles) {
  const imageMap = new Map();
  // MediaWiki API accepts up to 50 titles at once
  const BATCH = 50;

  for (let i = 0; i < titles.length; i += BATCH) {
    const batch = titles.slice(i, i + BATCH);
    const params = new URLSearchParams({
      action: "query",
      titles: batch.join("|"),
      prop: "pageimages",
      format: "json",
      pithumbsize: String(THUMB_SIZE),
    });

    const url = `${FANDOM_API}?${params}`;
    const resp = await fetch(url);
    if (!resp.ok) {
      console.error(`API error: ${resp.status} ${resp.statusText}`);
      continue;
    }

    const json = await resp.json();
    const pages = json.query?.pages ?? {};

    for (const page of Object.values(pages)) {
      if (page.thumbnail?.source) {
        // Normalize title for lookup
        imageMap.set(page.title.replace(/ /g, "_"), page.thumbnail.source);
      }
    }
  }

  return imageMap;
}

async function downloadImage(url, destPath) {
  const resp = await fetch(url);
  if (!resp.ok) {
    throw new Error(`HTTP ${resp.status} for ${url}`);
  }
  const buffer = Buffer.from(await resp.arrayBuffer());
  await writeFile(destPath, buffer);
}

async function downloadSet(idToWikiTitle, outDir, label, fallbacks = {}) {
  // Collect unique wiki titles
  const allTitles = [...new Set(Object.values(idToWikiTitle))];
  console.log(`\nFetching ${label} page images (${allTitles.length} pages)...`);

  const imageMap = await fetchPageImages(allTitles);

  let downloaded = 0;
  let skipped = 0;
  const missing = [];

  for (const [id, wikiTitle] of Object.entries(idToWikiTitle)) {
    const destPath = join(outDir, `${id}.png`);

    if (await fileExists(destPath)) {
      skipped++;
      continue;
    }

    const imageUrl = imageMap.get(wikiTitle) || fallbacks[id];
    if (!imageUrl) {
      missing.push({ id, wikiTitle });
      continue;
    }

    try {
      await downloadImage(imageUrl, destPath);
      downloaded++;
      process.stdout.write(".");
    } catch (err) {
      console.error(`\n  Failed to download ${id}: ${err.message}`);
      missing.push({ id, wikiTitle });
    }
  }

  console.log(`\n  ${label}: ${downloaded} downloaded, ${skipped} skipped (already exist)`);
  if (missing.length > 0) {
    console.log(`  Missing (${missing.length}):`);
    for (const m of missing) {
      console.log(`    - ${m.id} (wiki: ${m.wikiTitle})`);
    }
  }
}

async function main() {
  console.log("Grounded Wiki Image Downloader");
  console.log("==============================");
  console.log(`Output: public/images/bosses/ & public/images/creatures/`);

  await mkdir(BOSS_DIR, { recursive: true });
  await mkdir(CREATURE_DIR, { recursive: true });

  await downloadSet(BOSSES, BOSS_DIR, "Bosses");
  await downloadSet(CREATURES, CREATURE_DIR, "Creatures", WIKI_GG_FALLBACKS);

  console.log("\nDone!");
}

main().catch((err) => {
  console.error("Fatal error:", err);
  process.exit(1);
});
