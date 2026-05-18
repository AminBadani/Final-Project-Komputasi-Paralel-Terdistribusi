## Progress Log — [18 Mei 2026]

### Dikerjakan Hari Ini
- Implementasi komputasi paralel sesuai *rank* (*start* dan *end*) di masing-masing *workers*
- `MPI_Reduce()` untuk menggabungkan (menjumlahkan) seluruh hasil dari setiap *workers* 
- Menggabungkan kembali `master.h` ke dalam file `main.cpp`
- Fix bug: -
- Unit test: -

### Kendala / Blockers
- Deskripsi: `std::cout` pada *workers* lambat, terkadang output baru muncul pada perulangan berikutnya
- Solusi: -

### Rencana Besok
- Menyelesaikan masalah `std::cout` yang terlambat ditampilkan
- Impelementasi testing 

### Commit Hari Ini
- feat: Komputasi paralel pada *workers* dan `MPI_Reduce()`
- fix: -
- test: -