#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <vector>
#include <limits>
using namespace std;

const int MAX_USERS = 10;
const int MAX_PEGAWAI = 100;

struct Pegawai {
    string nama;
    string jabatan;
    string waktu_masuk;
    string waktu_keluar;
    string hasil_kerja;
    bool sudah_absen = false;
};

struct User {
    string username;
    string password;
    string alamat;
    bool isAdmin;
};

// Deklarasi fungsi
void registerUser(User users[], int &jumlah_user);
bool loginUser(User users[], int jumlah_user, User &loggedInUser);
void menuAdmin(Pegawai pegawai[], int &jumlah_pegawai, User &currentUser);
void menuUser(Pegawai pegawai[], int jumlah_pegawai, User &currentUser);
void tambahPegawai(Pegawai pegawai[], int &jumlah_pegawai);
void tampilkanPegawaiRekursif(Pegawai pegawai[], int index, int jumlah_pegawai);
void updatePegawai(Pegawai pegawai[], int jumlah_pegawai);
void hapusPegawai(Pegawai pegawai[], int &jumlah_pegawai);
void tampilkanPegawai(Pegawai pegawai[], int jumlah_pegawai);
void tampilkanPegawai(Pegawai pegawai[], int jumlah_pegawai, string nama);
void resetDataPegawai(Pegawai *pegawai, int *jumlah_pegawai);
void absenPegawai(Pegawai pegawai[], int jumlah_pegawai, User &currentUser);
void lihatHasilKerja(Pegawai pegawai[], int jumlah_pegawai);
void adminAbsenPegawai(Pegawai pegawai[], int jumlah_pegawai);
void sortNamaDescending(Pegawai pegawai[], int jumlah_pegawai);
void sortWaktuMasukAscending(Pegawai pegawai[], int jumlah_pegawai);
void sortPanjangNamaAscending(Pegawai pegawai[], int jumlah_pegawai);
int getValidInt(const string& prompt);
string getValidString(const string& prompt);
string getValidTime(const string& prompt);

int main() {
    User users[MAX_USERS] = {
        {"admin", "admin123", "Jl. Admin No.1", true},
        {"user1", "user1123", "Jl. User No.1", false},
        {"user2", "user2123", "Jl. User No.2", false}
    };
    Pegawai pegawai[MAX_PEGAWAI] = {
        {"Renaya", "Manager", "08:00", "17:00", "Menyelesaikan laporan keuangan", true},
        {"Rusdiana", "Staff", "09:00", "18:00", "Menginput data pelanggan", true},
        {"Niluh", "Supervisor", "08:30", "17:30", "Memeriksa pekerjaan staff", false}
    };
    int jumlah_user = 3, jumlah_pegawai = 3;
    int pilihan;
    User loggedInUser;

    while (true) {
        cout << "\n=== Sistem Manajemen Pegawai ===\n";
        cout << "1. Register\n2. Login\n3. Keluar\nPilihan: ";
        pilihan = getValidInt("Pilihan");

        switch (pilihan) {
            case 1:
                registerUser(users, jumlah_user);
                break;
            case 2:
                if (loginUser(users, jumlah_user, loggedInUser)) {
                    if (loggedInUser.isAdmin) {
                        menuAdmin(pegawai, jumlah_pegawai, loggedInUser);
                    } else {
                        menuUser(pegawai, jumlah_pegawai, loggedInUser);
                    }
                }
                break;
            case 3:
                cout << "Program berhenti.\n";
                return 0;
            default:
                cout << "Pilihan tidak valid!\n";
        }
    }
}

void registerUser(User users[], int &jumlah_user) {
    if (jumlah_user < MAX_USERS) {
        cout << "\n=== Register Akun ===\n";
        
        // Validasi username unik
        string username;
        bool usernameExists;
        do {
            usernameExists = false;
            username = getValidString("Masukkan Username");
            
            // Cek apakah username sudah ada
            for (int i = 0; i < jumlah_user; i++) {
                if (users[i].username == username) {
                    cout << "Username sudah digunakan. Silakan pilih username lain.\n";
                    usernameExists = true;
                    break;
                }
            }
        } while (usernameExists);
        
        users[jumlah_user].username = username;
        users[jumlah_user].password = getValidString("Masukkan Password");
        users[jumlah_user].alamat = getValidString("Masukkan Alamat");
        users[jumlah_user].isAdmin = false;
        jumlah_user++;
        cout << "Registrasi berhasil! Anda telah ditambahkan sebagai pengguna biasa, Silakan login.\n";
    } else {
        cout << "Jumlah user maksimal telah tercapai!\n";
    }
}

bool loginUser(User users[], int jumlah_user, User &loggedInUser) {
    string username, password;
    int attempt = 0;
    while (attempt < 3) {
        cout << "\n=== Login ===\n";
        username = getValidString("Masukkan Username");
        password = getValidString("Masukkan Password");
        
        for (int i = 0; i < jumlah_user; i++) {
            if (users[i].username == username && users[i].password == password) {
                loggedInUser = users[i];
                cout << "Login berhasil! Selamat datang, " << loggedInUser.username << ".\n";
                if (loggedInUser.isAdmin) {
                    cout << "Anda login sebagai Admin.\n";
                } else {
                    cout << "Anda login sebagai User.\n";
                }
                return true;
            }
        }
        cout << "Login gagal! Percobaan " << attempt+1 << " dari 3.\n";
        attempt++;
    }
    cout << "Anda gagal login 3 kali. Program berhenti.\n";
    exit(0);
}

void menuAdmin(Pegawai pegawai[], int &jumlah_pegawai, User &currentUser) {
    int pilihan;
    do {
        cout << "\n=== Menu Admin ===\n";
        cout << "1. Tambah Pegawai\n2. Tampilkan Pegawai\n3. Update Pegawai\n4. Hapus Pegawai\n";
        cout << "5. Sorting Data\n6. Absen Pegawai\n7. Reset Data\n8. Logout\nPilihan: ";
        pilihan = getValidInt("Pilihan");
        
        switch (pilihan) {
            case 1:
                tambahPegawai(pegawai, jumlah_pegawai);
                break;
            case 2: {
                int subPilihan;
                cout << "\n1. Tampilkan Semua Pegawai\n2. Cari Pegawai Berdasarkan Nama\nPilihan: ";
                subPilihan = getValidInt("Pilihan");
                if (subPilihan == 1) {
                    tampilkanPegawai(pegawai, jumlah_pegawai); 
                } else if (subPilihan == 2) {
                    string namaCari = getValidString("Masukkan nama pegawai yang dicari");
                    tampilkanPegawai(pegawai, jumlah_pegawai, namaCari);
                } else {
                    cout << "Pilihan tidak valid.\n";
                }
                break;
            }
            case 3:
                updatePegawai(pegawai, jumlah_pegawai);
                break;
            case 4:
                hapusPegawai(pegawai, jumlah_pegawai);
                break;
            case 5: {
                int pilihanSort;
                cout << "\n=== Menu Sorting ===\n";
                cout << "1. Sort Nama (Z-A)\n2. Sort Waktu Masuk (Ascending)\n3. Sort Panjang Nama (Pendek ke Panjang)\nPilihan: ";
                pilihanSort = getValidInt("Pilihan");
                switch (pilihanSort) {
                    case 1:
                        sortNamaDescending(pegawai, jumlah_pegawai);
                        cout << "Data pegawai telah diurutkan berdasarkan nama (Z-A).\n";
                        break;
                    case 2:
                        sortWaktuMasukAscending(pegawai, jumlah_pegawai);
                        cout << "Data pegawai telah diurutkan berdasarkan waktu masuk.\n";
                        break;
                    case 3:
                        sortPanjangNamaAscending(pegawai, jumlah_pegawai);
                        cout << "Data pegawai telah diurutkan berdasarkan panjang nama.\n";
                        break;
                    default:
                        cout << "Pilihan tidak valid!\n";
                }
                break;
            }
            case 6:
                adminAbsenPegawai(pegawai, jumlah_pegawai);
                break;
            case 7:
                resetDataPegawai(pegawai, &jumlah_pegawai);
                break;
            case 8:
                cout << "Logout berhasil.\n";
                break;
            default:
                cout << "Pilihan tidak valid!\n";
        }
    } while (pilihan != 8);
}

void menuUser(Pegawai pegawai[], int jumlah_pegawai, User &currentUser) {
    int pilihan;
    do {
        cout << "\n=== Menu User ===\n";
        cout << "1. Tampilkan Data Pegawai\n2. Absensi\n3. Lihat Hasil Kerja\n4. Logout\nPilihan: ";
        pilihan = getValidInt("Pilihan");
        
        switch (pilihan) {
            case 1: {
                int subPilihan;
                cout << "\n1. Tampilkan Semua Pegawai\n2. Cari Pegawai Berdasarkan Nama\nPilihan: ";
                subPilihan = getValidInt("Pilihan");
                if (subPilihan == 1) {
                    tampilkanPegawai(pegawai, jumlah_pegawai); 
                } else if (subPilihan == 2) {
                    string namaCari = getValidString("Masukkan nama pegawai yang dicari");
                    tampilkanPegawai(pegawai, jumlah_pegawai, namaCari);
                } else {
                    cout << "Pilihan tidak valid.\n";
                }
                break;
            }
            case 2:
                absenPegawai(pegawai, jumlah_pegawai, currentUser);
                break;
            case 3:
                lihatHasilKerja(pegawai, jumlah_pegawai);
                break;
            case 4:
                cout << "Logout berhasil.\n";
                break;
            default:
                cout << "Pilihan tidak valid!\n";
        }
    } while (pilihan != 4);
}

void tambahPegawai(Pegawai pegawai[], int &jumlah_pegawai) {
    if (jumlah_pegawai >= MAX_PEGAWAI) {
        cout << "Data pegawai penuh!\n";
        return;
    }

    cout << "\n=== Tambah Pegawai ===\n";
    string namaBaru;

    while (true) {
        try {
            namaBaru = getValidString("Nama Pegawai");

            for (int i = 0; i < jumlah_pegawai; i++) {
                if (pegawai[i].nama == namaBaru) {
                    throw runtime_error("Nama pegawai sudah ada. Masukkan nama yang berbeda.");
                }
            }

            break; // valid, keluar dari loop
        } catch (const exception &e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    pegawai[jumlah_pegawai].nama = namaBaru;
    pegawai[jumlah_pegawai].jabatan = getValidString("Jabatan Pegawai");
    pegawai[jumlah_pegawai].waktu_masuk = getValidTime("Waktu Masuk (format HH:MM)");
    pegawai[jumlah_pegawai].waktu_keluar = getValidTime("Waktu Keluar (format HH:MM)");
    pegawai[jumlah_pegawai].hasil_kerja = getValidString("Hasil Kerja");
    pegawai[jumlah_pegawai].sudah_absen = false;
    jumlah_pegawai++;
    cout << "Pegawai berhasil ditambahkan.\n";
}



void tampilkanPegawaiRekursif(Pegawai pegawai[], int index, int jumlah_pegawai) {
    if (index == 0) {
        cout << left << setw(5) << "No"
             << setw(20) << "Nama"
             << setw(20) << "Jabatan"
             << setw(15) << "Waktu Masuk"
             << setw(15) << "Waktu Keluar"
             << setw(20) << "Hasil Kerja"
             << setw(10) << "Status" << endl;
        cout << string(100, '-') << endl;
    }

    if (index >= jumlah_pegawai) return;

    cout << left << setw(5) << index + 1
         << setw(20) << pegawai[index].nama
         << setw(20) << pegawai[index].jabatan
         << setw(15) << pegawai[index].waktu_masuk
         << setw(15) << pegawai[index].waktu_keluar
         << setw(20) << (pegawai[index].hasil_kerja.empty() ? "-" : pegawai[index].hasil_kerja.substr(0, 15) + "...")
         << setw(10) << (pegawai[index].sudah_absen ? "Hadir" : "Belum") << endl;

    tampilkanPegawaiRekursif(pegawai, index + 1, jumlah_pegawai);
}

void updatePegawai(Pegawai pegawai[], int jumlah_pegawai) {
    if (jumlah_pegawai == 0) {
        cout << "Tidak ada data pegawai.\n";
        return;
    }

    tampilkanPegawai(pegawai, jumlah_pegawai);
    int idx = getValidInt("Masukkan nomor pegawai yang akan diupdate (0 untuk batal)");
    if (idx == 0) return;

    if (idx <= 0 || idx > jumlah_pegawai) {
        cout << "Nomor tidak valid!\n";
        return;
    }

    cout << "\n=== Update Pegawai ===\n";
    cout << "Kosongkan jika tidak ingin mengubah\n";

    string input;

    // Validasi nama baru
    try {
        input = getValidString("Nama Baru (" + pegawai[idx - 1].nama + ")");
        if (!input.empty()) {
            for (int i = 0; i < jumlah_pegawai; i++) {
                if (i != idx - 1 && pegawai[i].nama == input) {
                    throw runtime_error("Nama pegawai sudah digunakan oleh pegawai lain. Perubahan nama dibatalkan.");
                }
            }
            pegawai[idx - 1].nama = input;
        }
    } catch (const exception &e) {
        cout << "Error: " << e.what() << endl;
    }

    input = getValidString("Jabatan Baru (" + pegawai[idx - 1].jabatan + ")");
    if (!input.empty()) pegawai[idx - 1].jabatan = input;

    input = getValidTime("Waktu Masuk Baru (" + pegawai[idx - 1].waktu_masuk + ")");
    if (!input.empty()) pegawai[idx - 1].waktu_masuk = input;

    input = getValidTime("Waktu Keluar Baru (" + pegawai[idx - 1].waktu_keluar + ")");
    if (!input.empty()) pegawai[idx - 1].waktu_keluar = input;

    input = getValidString("Hasil Kerja Baru (" + pegawai[idx - 1].hasil_kerja + ")");
    if (!input.empty()) pegawai[idx - 1].hasil_kerja = input;

    cout << "Data pegawai berhasil diperbarui.\n";
}


void hapusPegawai(Pegawai pegawai[], int &jumlah_pegawai) {
    if (jumlah_pegawai == 0) {
        cout << "Data pegawai kosong.\n";
        return;
    }
    tampilkanPegawai(pegawai, jumlah_pegawai);
    int idx = getValidInt("Masukkan nomor pegawai yang akan dihapus (0 untuk batal)");
    if (idx == 0) return;
    
    if (idx > 0 && idx <= jumlah_pegawai) {
        char konfirmasi;
        cout << "Apakah Anda yakin ingin menghapus data " << pegawai[idx-1].nama << "? (y/n): ";
        cin >> konfirmasi;
        cin.ignore();
        
        if (konfirmasi == 'y' || konfirmasi == 'Y') {
            for (int i = idx - 1; i < jumlah_pegawai - 1; i++) {
                pegawai[i] = pegawai[i + 1];
            }
            jumlah_pegawai--;
            cout << "Data berhasil dihapus.\n";
        } else {
            cout << "Penghapusan dibatalkan.\n";
        }
    } else {
        cout << "Nomor tidak valid!\n";
    }
}

void tampilkanPegawai(Pegawai pegawai[], int jumlah_pegawai) {
    if (jumlah_pegawai == 0) {
        cout << "Tidak ada data pegawai.\n";
        return;
    }
    
    cout << "\n=== Data Pegawai ===\n";
    tampilkanPegawaiRekursif(pegawai, 0, jumlah_pegawai);
}

void tampilkanPegawai(Pegawai pegawai[], int jumlah_pegawai, string nama) {
    bool ditemukan = false;
    cout << "\n=== Hasil Pencarian Pegawai ===\n";
    cout << "Mencari pegawai bernama: " << nama << endl;
    
    for (int i = 0; i < jumlah_pegawai; i++) {
        if (pegawai[i].nama.find(nama) != string::npos) {
            cout << i + 1 << ". " << pegawai[i].nama 
                 << " (" << pegawai[i].jabatan << ")"
                 << ", Masuk: " << pegawai[i].waktu_masuk
                 << ", Keluar: " << pegawai[i].waktu_keluar
                 << ", Hasil: " << (pegawai[i].hasil_kerja.empty() ? "-" : pegawai[i].hasil_kerja.substr(0, 20) + "...")
                 << ", Status: " << (pegawai[i].sudah_absen ? "Hadir" : "Belum") << endl;
            ditemukan = true;
        }
    }
    
    if (!ditemukan) {
        cout << "Pegawai dengan nama '" << nama << "' tidak ditemukan.\n";
    }
}

void resetDataPegawai(Pegawai *pegawai, int *jumlah_pegawai) {
    char konfirmasi;
    cout << "Apakah Anda yakin ingin mereset semua data pegawai? (y/n): ";
    cin >> konfirmasi;
    cin.ignore();
    if (konfirmasi == 'y' || konfirmasi == 'Y') {
        *jumlah_pegawai = 0;
        cout << "Seluruh data pegawai berhasil di-reset.\n";
    } else {
        cout << "Reset dibatalkan.\n";
    }
}

void absenPegawai(Pegawai pegawai[], int jumlah_pegawai, User &currentUser) {
    bool ditemukan = false;
    for (int i = 0; i < jumlah_pegawai; i++) {
        if (pegawai[i].nama == currentUser.username) {
            if (pegawai[i].sudah_absen) {
                cout << "Anda sudah melakukan absensi hari ini.\n";
            } else {
                pegawai[i].sudah_absen = true;
                cout << "Absensi berhasil. Selamat bekerja, " << currentUser.username << "!\n";
            }
            ditemukan = true;
            break;
        }
    }
    
    if (!ditemukan) {
        cout << "Anda tidak terdaftar sebagai pegawai. Silakan hubungi admin.\n";
    }
}

void adminAbsenPegawai(Pegawai pegawai[], int jumlah_pegawai) {
    if (jumlah_pegawai == 0) {
        cout << "Tidak ada data pegawai.\n";
        return;
    }
    
    tampilkanPegawai(pegawai, jumlah_pegawai);
    int idx = getValidInt("Masukkan nomor pegawai yang akan diabsensi (0 untuk batal)");
    if (idx == 0) return;
    
    if (idx > 0 && idx <= jumlah_pegawai) {
        if (pegawai[idx-1].sudah_absen) {
            cout << "Pegawai " << pegawai[idx-1].nama << " sudah melakukan absensi.\n";
        } else {
            pegawai[idx-1].sudah_absen = true;
            cout << "Absensi berhasil. Pegawai " << pegawai[idx-1].nama << " telah dicatat hadir.\n";
        }
    } else {
        cout << "Nomor tidak valid!\n";
    }
}

void lihatHasilKerja(Pegawai pegawai[], int jumlah_pegawai) {
    if (jumlah_pegawai == 0) {
        cout << "Tidak ada data pegawai.\n";
        return;
    }
    
    cout << "\n=== Hasil Kerja Pegawai ===\n";
    for (int i = 0; i < jumlah_pegawai; i++) {
        cout << i+1 << ". " << pegawai[i].nama << " (" << pegawai[i].jabatan << "):\n";
        cout << "   - " << (pegawai[i].hasil_kerja.empty() ? "Belum ada laporan hasil kerja" : pegawai[i].hasil_kerja) << "\n\n";
    }
}

void sortNamaDescending(Pegawai pegawai[], int jumlah_pegawai) {
    for (int i = 0; i < jumlah_pegawai - 1; i++) {
        for (int j = i + 1; j < jumlah_pegawai; j++) {
            if (pegawai[i].nama < pegawai[j].nama) {
                swap(pegawai[i], pegawai[j]);
            }
        }
    }
}

void sortWaktuMasukAscending(Pegawai pegawai[], int jumlah_pegawai) {
    auto convertToMinutes = [](const string& waktu) {
        if (waktu.length() < 4) return 0;
        int jam = stoi(waktu.substr(0,2));
        int menit = stoi(waktu.substr(3,2));
        return jam * 60 + menit;
    };

    for (int i = 0; i < jumlah_pegawai - 1; i++) {
        for (int j = i + 1; j < jumlah_pegawai; j++) {
            if (convertToMinutes(pegawai[i].waktu_masuk) > convertToMinutes(pegawai[j].waktu_masuk)) {
                swap(pegawai[i], pegawai[j]);
            }
        }
    }
}

void sortPanjangNamaAscending(Pegawai pegawai[], int jumlah_pegawai) {
    for (int i = 0; i < jumlah_pegawai - 1; i++) {
        for (int j = i + 1; j < jumlah_pegawai; j++) {
            if (pegawai[i].nama.length() > pegawai[j].nama.length()) {
                swap(pegawai[i], pegawai[j]);
            }
        }
    }
}

int getValidInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt << ": ";
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input tidak valid. Harap masukkan angka.\n";
        } else {
            cin.ignore();
            return value;
        }
    }
}

string getValidString(const string& prompt) {
    string input;
    while (true) {
        cout << prompt << ": ";
        getline(cin, input);
        if (input.empty()) {
            cout << "Input tidak boleh kosong.\n";
        } else {
            return input;
        }
    }
}

string getValidTime(const string& prompt) {
    string input;
    while (true) {
        cout << prompt << ": ";
        getline(cin, input);
        
        if (input.empty()) return input;
        
        // Validasi format waktu HH:MM
        if (input.length() == 5 && 
            isdigit(input[0]) && isdigit(input[1]) && 
            input[2] == ':' && 
            isdigit(input[3]) && isdigit(input[4])) {
            
            int jam = stoi(input.substr(0,2));
            int menit = stoi(input.substr(3,2));
            
            if (jam >= 0 && jam < 24 && menit >= 0 && menit < 60) {
                return input;
            }
        }
        
        cout << "Format waktu tidak valid. Gunakan format HH:MM (contoh: 08:00)\n";
    }
}