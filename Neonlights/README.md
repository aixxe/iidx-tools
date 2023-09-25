## Neonlights

A bunch of quickly hacked together patches and hooks to restore normal rates for cab lighting with 120Hz hex edits.

I don't remember what any of this does, but it seems to work.

## Requirements

The library offsets are specific to the **2023090500** (LDJ-003) game version.

Patterns are included next to each offset, so it should be easy to find and update as needed.

### Installation

Extract `Neonlights.dll` to your game directory and amend your launch command.

#### [Bemanitools](https://github.com/djhackersdev/bemanitools)

```
launcher.exe [...] -K Neonlights.dll
```

#### [spice2x](https://spice2x.github.io)

```
spice64.exe [...] -k Neonlights.dll
```