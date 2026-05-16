## Progress Log — [16 Mei 2026]

### Dikerjakan Hari Ini
- Update konfigurasi `DockerFile`
- Fix bug: Menonaktifkan password ketika `ssh` ke workers dan memperbolehkan `root` untuk `mpirun`
- Unit test: -

### Kendala / Blockers
- Deskripsi: 
    1. Perlu password untuk `ssh` ke workers
    2. Tidak bisa `mpirun` sebagai `root`
- Solusi: 
    1. Menambahkan `StrictHostKeyChecking no` pada konfigurasi `ssh` dan konfigurasi PAM Ubuntu `usePAM no`
    2.  Menambahkan `OMPI_ALLOW_RUN_AS_ROOT=1` dan `OMPI_ALLOW_RUN_AS_ROOT_CONFIRM=1`

### Rencana Besok
- Implementasi MPI dari master ke worker
- Send/Scatter ke workers

### Commit Hari Ini
- feat: -
- fix: koneksi `ssh` dan jalankan `mpirun`
- test: -