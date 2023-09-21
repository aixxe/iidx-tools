meta:
  id: one
  application: beatmania IIDX
  file-extension: .1
  endian: le
doc-ref: https://github.com/SaxxonPike/rhythm-game-formats/blob/master/iidx/1.md

seq:
  - id: sph
    type: dirent # 0
  - id: spn
    type: dirent # 1
  - id: spa
    type: dirent # 2
  - id: spb
    type: dirent # 3
  - id: spl
    type: dirent # 4
  - size: 8      # 5
  - id: dph
    type: dirent # 6
  - id: dpn
    type: dirent # 7
  - id: dpa
    type: dirent # 8
  - size: 8      # 9
  - id: dpl
    type: dirent # 10
  - size: 8      # 11
  - size: 8      # 12

types:
  dirent:
    seq:
      - id: offset
        type: s4
      - id: size
        type: s4
    instances:
      chart:
        pos: offset
        size: size
        type: chart
        if: size > 0
  chart:
    seq:
      - id: events
        type: event
        repeat: eos
  event:
    seq:
      - id: offset
        type: s4
      - id: type
        type: u1
        enum: type
      - id: event
        type:
          switch-on: type
          cases:
            type::p1_note: note
            type::p2_note: note
            type::p1_sample: sample
            type::p2_sample: sample
            type::tempo: tempo
            type::meter: meter
            type::end: end
            type::bgm: bgm
            type::timing: timing
            type::measure: measure
            type::count: count
  # Visible Note (00, 01)
  note:
    seq:
      - id: column
        type: u1
        enum: column
      - id: duration
        type: s2
  # Sample Change (02, 03)
  sample:
    seq:
      - id: column
        type: u1
        enum: column
      - id: sample
        type: s2
  # Tempo Change (04)
  tempo:
    seq:
      - id: denominator
        type: u1
      - id: numerator
        type: s2
  # Meter Information (05)
  meter:
    seq:
      - id: denominator
        type: u1
      - id: numerator
        type: s2
  # End of Song (06)
  end:
    seq:
      - id: parameter
        type: u1
      - id: value
        type: s2
  # BGM Sound (07)
  bgm:
    seq:
      - id: stereo
        type: u1
      - id: sound
        type: s2
  # Timing Window Information (08)
  timing:
    seq:
      - id: window
        type: u1
        enum: window
      - id: frames
        type: s2
  # Measure Bar (0C)
  measure:
    seq:
      - id: player
        type: u1
        enum: player
      - id: value
        type: s2
  # Note Count Information (10)
  count:
    seq:
      - id: player
        type: u1
        enum: player
      - id: count
        type: s2

enums:
  type:
    0: p1_note
    1: p2_note
    2: p1_sample
    3: p2_sample
    4: tempo
    5: meter
    6: end
    7: bgm
    8: timing
    12: measure
    16: count
  player:
    0: p1
    1: p2
  column:
    0: key_1
    1: key_2
    2: key_3
    3: key_4
    4: key_5
    5: key_6
    6: key_7
    7: scratch
  window:
    0: late_end
    1: late_good
    2: late_great
    3: early_great
    4: early_good
    5: early_end
