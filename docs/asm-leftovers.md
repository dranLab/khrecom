# Parked INCLUDE_ASM functions and asm data

Units whose remaining functions (or data) hit the time box of the leftovers plan. Each entry: what remains and
the closest result. Revisit after the units with few leftovers are done; lessons from later units may unlock these.

| Unit | Function / data | Remaining difference |
|---|---|---|
| Game/Collision/Ride | `.rodata` from asm | Split puts the unit's `.rodata` at 4 mod 8 (starts with another unit's vtable tail); gcc 8-aligns strings, so source data can't line up. Needs a split change (out of scope). |
| System/dlmalloc | `destroy_mspace` | Original: `lw seg.next; beqz; lw; bnezl; lw; L: bnezl; lw` (peeled step + while loop with a duplicated exit test). jump2 cross-jumps the peeled `lw; beqz` into the loop; an asm statement between them stops that but reorg then leaves the second test's delay slot unfilled. See the note in the source. |
| System/Graphic/2d/2DCard | `replaceNorma` | Original loads the by-value `NormaSrc` as four `lhu` + `dsll`/`or` into a1; ours copies it to an aligned temporary and uses one `ld`. No struct shape or compiler version reproduces it. |
| Game/Gimmick/GimmickPlantParent | `beginStateHitAttack` | `sb m_Stat` is emitted before `li a1,1` for the `saveStat(1)` call; the original has the store last (delay slot). Both depend only on the call, so sched1's dying-source rule orders them; 58 spellings/orders tried. ~20 min over two rounds (stopped at the user's request, not the 45-min box). See the note in the source. |
| Game/Enemy/EnemyWhitemushroom | `damage_` | Case 4: the original loads the vptr for `getLockOnPos` right after setupFade's two TIMEQ stores. Ours loads it after all six stores, because the vtable-entry loads (unknown base) depend on every store. All 720 store orders (several flag forms, inline and written out), reference stores, asm fillers/barriers and local placement were tried. `-fstrict-aliasing` moves the load (an aliasing question), but that is not the build flag. ~45 min. Data comes from source (placeholder + jump table asm). |
| System/Graphic/Effect/libppp/keProg/pppKeShpTail3 | `pppKeShpTail3Draw` | 37 lines, all in one region: between the ScreenMatrix copy and the CMat asm the original addresses the stack-resident `pw` through `$a3` (`move a3,sp`, a copy of the soft frame pointer = &mat0). A dummy `mat0` pointer reproduces the copy, but CSE keeps the fixed frame register canonical, so `pw` never uses it. Near-match (with hacks) in the source. ~45 min (plus hill climbs and random mutation search). |
