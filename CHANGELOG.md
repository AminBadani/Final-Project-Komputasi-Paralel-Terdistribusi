## Progress Log — [2 Juni 2026]

### Dikerjakan Hari Ini
- Memperbaiki perhitungan durasi global (`duration_global`) dan menambahkan durasi lokal (`duration_local`)
    - `duration_global = MPI_WTime()`
    - `duration_local = omp_get_wtime()`
- Menambahkan beberapa test *edge case*
    - `start = 0` dan `end = 0`
    - `start = -10` dan `end = 5`
    - `start = 10` dan `end = 5`
    - `active_cores <= 0`
    - `active_cores > limit `
- Fix bug: -
- Unit test: akurasi; edge case; skalabilitas;

### Kendala / Blockers
- Deskripsi: -
- Solusi: -

### Rencana Besok
- Implementasi *live progress* setiap `n` persen
- Implementasi *checkpoint*: replikasi `local_sum`, *send* dari *workers* ke *master* setiap `n` detik

### Commit Hari Ini
- feat: 
- fix: 
- test: akurasi (PASS); edge case (PASS); skalabilitas (PASS);