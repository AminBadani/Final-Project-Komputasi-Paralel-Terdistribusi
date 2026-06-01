## Progress Log — [1 Juni 2026]

### Dikerjakan Hari Ini
- Perubahan struktur kode program;
    - menghilangkan menu dan mengganti dengan input *argument* `total_numbers = argv[1]` dan `active_workers = argv[2]` sebelum *running*
    - menambahkan `send-receive` hasil dari *workers* ke *master* 
- Fix bug: menyelesaikan *bug* `std::cout` dari *workers* yang terlambat di tampilkan
- Unit test: akurasi; skalabilitas;

### Kendala / Blockers
- Deskripsi: -
- Solusi: -

### Rencana Besok
- Implementasi testing *edge case*
- Implementasi checkpoint: replikasi `local_sum`, *send* dari *workers* ke *master* setiap `n` detik

### Commit Hari Ini
- feat: Perubahan struktur program dengan menghilangkan menu
- fix: Memperbaiki `std::cout` hasil *workers* yang terlambat ditampilkan
- test: akurasi (PASS); skalabilitas (PASS);