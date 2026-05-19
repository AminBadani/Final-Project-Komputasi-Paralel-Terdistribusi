## Progress Log — [19 Mei 2026]

### Dikerjakan Hari Ini
- Impelementasi testing sederhana untuk test akurasi (static = hasil kalkulasi) dan test scalabilitas (core = 1 | cores = 4)
- Fix bug: -
- Unit test: akurasi; skalabilitas;

### Kendala / Blockers
- Deskripsi: 
    1. `std::cout` pada *workers* lambat, terkadang output baru muncul pada perulangan berikutnya
    2. Belum ada test *edge case* (null, 0, negatif)
- Solusi: -

### Rencana Besok
- Menyelesaikan masalah `std::cout` yang terlambat ditampilkan
- Implementasi testing *edge case*
- Membuat mekanisme untuk tetap melanjutkan program ketika tipe data yang diisi ke dalam input tidak sesuai 

### Commit Hari Ini
- feat: Implementasi test akurasi dan tes skalabilitas
- fix: -
- test: akurasi (PASS) dan skalabilitas (PASS)