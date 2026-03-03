export interface BossData {
  id: string;
  flag: string;
  name: string;
  location: string;
}

export const BOSSES: BossData[] = [
  {
    id: "killbroodmother",
    flag: "KillBroodmother",
    name: "Hedge Broodmother",
    location: "Hedge Lab",
  },
  {
    id: "killinfectedbroodmother",
    flag: "KillInfectedBroodmother",
    name: "Infected Broodmother",
    location: "Haze Lab",
  },
  {
    id: "killwaspqueen",
    flag: "KillWaspQueen",
    name: "Wasp Queen",
    location: "Upper Yard",
  },
  {
    id: "killdirector",
    flag: "KillDirector",
    name: "Director Schmector",
    location: "Castle Moldorc",
  },
  {
    id: "killmantis",
    flag: "KillMantis",
    name: "Mantis",
    location: "Mantis Den",
  },
];
