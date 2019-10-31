# ArenaQuickClose

Reduce downtime between Arena matches by forcing CPUs to join early.

## Usage

As the host of an online arena lobby, press <kbd>VEFX</kbd> + <kbd>EFFECT</kbd> during matching.

The lobby will be closed and any remaining player slots will be populated with CPUs.

## Install

Build from source or [download a pre-compiled version][1], extract to the game directory and add a hook entry to your game launching script. _Syntax may differ depending on toolset used._

>launcher -K iidxhook.dll **-K ArenaQuickClose.dll** bm2dx.dll %*

[1]: https://github.com/aixxe/iidx-tools/raw/master/ArenaQuickClose/bin/ArenaQuickClose.dll

## Requirements

The library offsets are specific to the **2019090200** game version.