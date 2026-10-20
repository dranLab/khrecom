# Parked INCLUDE_ASM functions and asm data

Units whose remaining functions (or data) hit the time box of the leftovers plan. Each entry: what remains and
the closest result. Revisit after the units with few leftovers are done; lessons from later units may unlock these.

| Unit | Function / data | Remaining difference |
|---|---|---|
| Game/Collision/Ride | `.rodata` from asm | Split puts the unit's `.rodata` at 4 mod 8 (starts with another unit's vtable tail); gcc 8-aligns strings, so source data can't line up. Needs a split change (out of scope). |
| System/dlmalloc | `destroy_mspace` | Original: `lw seg.next; beqz; lw; bnezl; lw; L: bnezl; lw` (peeled step + while loop with a duplicated exit test). jump2 cross-jumps the peeled `lw; beqz` into the loop; an asm statement between them stops that but reorg then leaves the second test's delay slot unfilled. See the note in the source. |
| System/Graphic/2d/2DCard | `replaceNorma` | Original loads the by-value `NormaSrc` as four `lhu` + `dsll`/`or` into a1; ours copies it to an aligned temporary and uses one `ld`. No struct shape or compiler version reproduces it. |
| Game/Collision/CollUtilA | `detect2DLineLine` | Final 4/1 choice is a `movn` (jump-pass conditional move) where the original branches (`beqz s5; li v0,4` / `b; li v0,1`); the `\|\|`-test form avoids the movn but mirrors the arms. See the note in the source. |
| Game/Gimmick/GimmickPlantParent | `beginStateHitAttack` | Before the `saveStat(1)` call ours emits `sb m_Stat` before `li a1,1` (the `li` lands in the delay slot); the original has the store last. Same-cycle issue, order from sched1's dying-source rule; no source form found. See the note in the source. |
