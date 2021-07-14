***2021-07-14T10:29:13-07:00***

Run on (6 X 4008 MHz CPU s)

CPU Caches:
  - L1 Data 32 KiB (x3)
  - L1 Instruction 32 KiB (x3)
  - L2 Unified 256 KiB (x3)
  - L3 Unified 12288 KiB (x1)

Benchmark | Time(set) | CPU(set) | Iter(set) | Time(unordered_set) | CPU(unordered_set) | Iter(unordered_set) | Time(skip_list) | CPU(skip_list) | Iter(skip_list)
--- | --- | --- | --- |--- |--- |--- |---  |---  |---
FindRandom0In0 | 72.8 ns | 71.5 ns | 8960000 | 139 ns | 140 ns | 5600000 | 2.21 ns | 2.20 ns | 320000000
FindRandom10In1 | 169 ns | 167 ns | 4480000 | 235 ns | 235 ns | 2986667 | 310 ns | 311 ns | 2357895
FindRandom80In8 | 1024 ns | 1025 ns | 640000 | 946 ns | 942 ns | 746667 | 2364 ns | 2354 ns | 298667
FindRandom640In64 | 12888 ns | 12835 ns | 56000 | 9928 ns | 9766 ns | 64000 | 21761 ns | 21484 ns | 32000
FindRandom5120In512 | 108254 ns | 107422 ns | 6400 | 79348 ns | 78474 ns | 8960 | 185953 ns | 187976 ns | 4073
FindRandom40960In4096 | 864843 ns | 857597 ns | 747 | 543480 ns | 544085 ns | 1120 | 1664592 ns | 1650799 ns | 407
FindRandom327680In32768 | 8851834 ns | 8854167 ns | 90 | 4378638 ns | 4329819 ns | 166 | 18324703 ns | 18158784 ns | 37

**First benchmark conclusion:** *skip_list* is about 4 times slower than *unordered_set*, and about 2 times slower than *set* on 32k random inserts and 320k random finds.
