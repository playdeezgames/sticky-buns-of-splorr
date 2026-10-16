package sbos

import "core:math/rand"

// Inclusive on both ends, like RNG::FromRange.
rng_range :: proc(low, high: int) -> int {
	return low + rand.int_max(high - low + 1)
}

Weighted :: struct($T: typeid) {
	value:  T,
	weight: int,
}

// Replaces RNG::FromGenerator. Entries are walked in slice order (C++ walked map key order).
rng_weighted :: proc(entries: []Weighted($T)) -> T {
	total := 0
	for e in entries do total += e.weight
	n := rng_range(0, total - 1)
	for e in entries {
		if e.weight > n do return e.value
		n -= e.weight
	}
	panic("dint work!")
}
