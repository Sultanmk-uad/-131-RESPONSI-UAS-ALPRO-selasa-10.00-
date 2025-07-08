#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Transaksi {
    string kodeProduk;
    string namaProduk;
    int jumlahDibeli;
    double harga;
    double totalPendapatan;
};

class AnalisisPenjualan {
private:
    Transaksi* dataTransaksi;
    int jumlahTransaksi;
    int kapasitas;

public:

    AnalisisPenjualan(int kapasitasAwal = 100) {
        kapasitas = kapasitasAwal;
        dataTransaksi = new Transaksi[kapasitas];
        jumlahTransaksi = 0;
    }


    ~AnalisisPenjualan() {
        delete[] dataTransaksi;
    }


    bool bacaDataDariFile(const string& namaFile) {
        ifstream file(namaFile);
        if (!file.is_open()) {
            cout << "Error: Tidak dapat membuka file " << namaFile << endl;
            return false;
        }

        jumlahTransaksi = 0;
        string line;
        
        while (getline(file, line) && jumlahTransaksi < kapasitas) {

            size_t pos = 0;
            string token;
            int fieldCount = 0;
            

            Transaksi temp;
            
            while ((pos = line.find(',')) != string::npos) {
                token = line.substr(0, pos);
                

                token.erase(0, token.find_first_not_of(" \t"));
                token.erase(token.find_last_not_of(" \t") + 1);
                
                switch (fieldCount) {
                    case 0: temp.kodeProduk = token; break;
                    case 1: temp.namaProduk = token; break;
                    case 2: temp.jumlahDibeli = stoi(token); break;
                    case 3: temp.harga = stod(token); break;
                }
                
                line.erase(0, pos + 1);
                fieldCount++;
            }
            

            if (fieldCount == 3) {
                line.erase(0, line.find_first_not_of(" \t"));
                line.erase(line.find_last_not_of(" \t") + 1);
                temp.harga = stod(line);
            }
            

            temp.totalPendapatan = temp.jumlahDibeli * temp.harga;
            

            dataTransaksi[jumlahTransaksi] = temp;
            jumlahTransaksi++;
        }
        
        file.close();
        cout << "Berhasil membaca " << jumlahTransaksi << " transaksi dari file." << endl;
        return true;
    }


    int cariBarangByKode(const string& kode) {
        for (int i = 0; i < jumlahTransaksi; i++) {
            if (dataTransaksi[i].kodeProduk == kode) {
                return i;
            }
        }
        return -1; // Tidak ditemukan
    }

    int cariBarangTermurah() {
        if (jumlahTransaksi == 0) return -1;
        
        int indexTermurah = 0;
        for (int i = 1; i < jumlahTransaksi; i++) {
            if (dataTransaksi[i].harga < dataTransaksi[indexTermurah].harga) {
                indexTermurah = i;
            }
        }
        return indexTermurah;
    }


    void urutkanTermahalKeTermurah() {
        for (int i = 0; i < jumlahTransaksi - 1; i++) {
            for (int j = 0; j < jumlahTransaksi - i - 1; j++) {
                if (dataTransaksi[j].harga < dataTransaksi[j + 1].harga) {
                    // Swap
                    Transaksi temp = dataTransaksi[j];
                    dataTransaksi[j] = dataTransaksi[j + 1];
                    dataTransaksi[j + 1] = temp;
                }
            }
        }
    }

    void urutkanTermurahKeTermahal() {
        for (int i = 0; i < jumlahTransaksi - 1; i++) {
            for (int j = 0; j < jumlahTransaksi - i - 1; j++) {
                if (dataTransaksi[j].harga > dataTransaksi[j + 1].harga) {
                   
                    Transaksi temp = dataTransaksi[j];
                    dataTransaksi[j] = dataTransaksi[j + 1];
                    dataTransaksi[j + 1] = temp;
                }
            }
        }
    }

    double hitungTotalPendapatanProduk(const string& kode) {
        double total = 0;
        for (int i = 0; i < jumlahTransaksi; i++) {
            if (dataTransaksi[i].kodeProduk == kode) {
                total += dataTransaksi[i].totalPendapatan;
            }
        }
        return total;
    }

    double hitungTotalSeluruhPendapatan() {
        double total = 0;
        for (int i = 0; i < jumlahTransaksi; i++) {
            total += dataTransaksi[i].totalPendapatan;
        }
        return total;
    }

    bool simpanHasilAnalisis(const string& namaFile) {
        ofstream file(namaFile);
        if (!file.is_open()) {
            cout << "Error: Tidak dapat membuat file " << namaFile << endl;
            return false;
        }

        file << "=== HASIL ANALISIS PENJUALAN ===" << endl;
        file << "Kode Produk\tNama Produk\t\tJumlah\tHarga\t\tTotal Pendapatan" << endl;
        file << "========================================================================" << endl;
        
        for (int i = 0; i < jumlahTransaksi; i++) {
            file << dataTransaksi[i].kodeProduk << "\t\t"
                 << dataTransaksi[i].namaProduk << "\t\t"
                 << dataTransaksi[i].jumlahDibeli << "\t"
                 << fixed << setprecision(2) << dataTransaksi[i].harga << "\t\t"
                 << dataTransaksi[i].totalPendapatan << endl;
        }
        
        file << "========================================================================" << endl;
        file << "TOTAL SELURUH PENDAPATAN: " << fixed << setprecision(2) 
             << hitungTotalSeluruhPendapatan() << endl;
        
        file.close();
        cout << "Hasil analisis berhasil disimpan ke " << namaFile << endl;
        return true;
    }

    bool simpanHasilSorting(const string& namaFile, const string& jenisSort) {
        ofstream file(namaFile);
        if (!file.is_open()) {
            cout << "Error: Tidak dapat membuat file " << namaFile << endl;
            return false;
        }

        file << "=== HASIL SORTING PENJUALAN (" << jenisSort << ") ===" << endl;
        file << "Kode Produk\tNama Produk\t\tJumlah\tHarga\t\tTotal Pendapatan" << endl;
        file << "========================================================================" << endl;
        
        for (int i = 0; i < jumlahTransaksi; i++) {
            file << dataTransaksi[i].kodeProduk << "\t\t"
                 << dataTransaksi[i].namaProduk << "\t\t"
                 << dataTransaksi[i].jumlahDibeli << "\t"
                 << fixed << setprecision(2) << dataTransaksi[i].harga << "\t\t"
                 << dataTransaksi[i].totalPendapatan << endl;
        }
        
        file.close();
        cout << "Hasil sorting berhasil disimpan ke " << namaFile << endl;
        return true;
    }

    void tampilkanData() {
        cout << "\n=== DATA TRANSAKSI ===" << endl;
        cout << "Kode\tNama Produk\t\tJumlah\tHarga\t\tTotal" << endl;
        cout << "========================================================" << endl;
        
        for (int i = 0; i < jumlahTransaksi; i++) {
            cout << dataTransaksi[i].kodeProduk << "\t"
                 << dataTransaksi[i].namaProduk << "\t\t"
                 << dataTransaksi[i].jumlahDibeli << "\t"
                 << fixed << setprecision(2) << dataTransaksi[i].harga << "\t\t"
                 << dataTransaksi[i].totalPendapatan << endl;
        }
    }

    void tampilkanDetailProduk(int index) {
        if (index >= 0 && index < jumlahTransaksi) {
            cout << "\n=== DETAIL PRODUK ===" << endl;
            cout << "Kode Produk: " << dataTransaksi[index].kodeProduk << endl;
            cout << "Nama Produk: " << dataTransaksi[index].namaProduk << endl;
            cout << "Jumlah Dibeli: " << dataTransaksi[index].jumlahDibeli << endl;
            cout << "Harga: " << fixed << setprecision(2) << dataTransaksi[index].harga << endl;
            cout << "Total Pendapatan: " << dataTransaksi[index].totalPendapatan << endl;
        }
    }

    int getJumlahTransaksi() const {
        return jumlahTransaksi;
    }
};

void tampilkanMenu() {
    cout << "\n=== PROGRAM ANALISIS PENJUALAN ===" << endl;
    cout << "1. Baca data dari file" << endl;
    cout << "2. Tampilkan semua data" << endl;
    cout << "3. Cari barang berdasarkan kode" << endl;
    cout << "4. Cari barang termurah" << endl;
    cout << "5. Urutkan produk termahal ke termurah" << endl;
    cout << "6. Urutkan produk termurah ke termahal" << endl;
    cout << "7. Hitung total pendapatan per produk" << endl;
    cout << "8. Hitung total seluruh pendapatan" << endl;
    cout << "9. Simpan hasil analisis" << endl;
    cout << "10. Simpan hasil sorting" << endl;
    cout << "0. Keluar" << endl;
    cout << "Pilih menu: ";
}

int main() {
    AnalisisPenjualan analisis;
    int pilihan;
    string namaFile, kode;
    
    cout << "=== PROGRAM ANALISIS PENJUALAN TOKO ===" << endl;
    cout << "Dibuat untuk membantu analisis data penjualan" << endl;
    
    do {
        tampilkanMenu();
        cin >> pilihan;
        
        switch (pilihan) {
            case 1: {
                cout << "Masukkan nama file input (contoh: data_penjualan.txt): ";
                cin >> namaFile;
                analisis.bacaDataDariFile(namaFile);
                break;
            }
            
            case 2: {
                if (analisis.getJumlahTransaksi() == 0) {
                    cout << "Tidak ada data. Silakan baca data dari file terlebih dahulu." << endl;
                } else {
                    analisis.tampilkanData();
                }
                break;
            }
            
            case 3: {
                cout << "Masukkan kode produk yang dicari: ";
                cin >> kode;
                int index = analisis.cariBarangByKode(kode);
                if (index != -1) {
                    cout << "Produk ditemukan!" << endl;
                    analisis.tampilkanDetailProduk(index);
                } else {
                    cout << "Produk dengan kode " << kode << " tidak ditemukan." << endl;
                }
                break;
            }
            
            case 4: {
                int index = analisis.cariBarangTermurah();
                if (index != -1) {
                    cout << "Produk termurah:" << endl;
                    analisis.tampilkanDetailProduk(index);
                } else {
                    cout << "Tidak ada data produk." << endl;
                }
                break;
            }
            
            case 5: {
                analisis.urutkanTermahalKeTermurah();
                cout << "Data berhasil diurutkan dari termahal ke termurah." << endl;
                analisis.tampilkanData();
                break;
            }
            
            case 6: {
                analisis.urutkanTermurahKeTermahal();
                cout << "Data berhasil diurutkan dari termurah ke termahal." << endl;
                analisis.tampilkanData();
                break;
            }
            
            case 7: {
                cout << "Masukkan kode produk: ";
                cin >> kode;
                double total = analisis.hitungTotalPendapatanProduk(kode);
                if (total > 0) {
                    cout << "Total pendapatan produk " << kode << ": " 
                         << fixed << setprecision(2) << total << endl;
                } else {
                    cout << "Produk tidak ditemukan atau tidak ada penjualan." << endl;
                }
                break;
            }
            
            case 8: {
                double total = analisis.hitungTotalSeluruhPendapatan();
                cout << "Total seluruh pendapatan: " << fixed << setprecision(2) << total << endl;
                break;
            }
            
            case 9: {
                cout << "Masukkan nama file output (contoh: hasil_analisis.txt): ";
                cin >> namaFile;
                analisis.simpanHasilAnalisis(namaFile);
                break;
            }
            
            case 10: {
                cout << "Pilih jenis sorting:" << endl;
                cout << "1. Termahal ke Termurah" << endl;
                cout << "2. Termurah ke Termahal" << endl;
                cout << "Pilih: ";
                int jenisSorting;
                cin >> jenisSorting;
                
                if (jenisSorting == 1) {
                    analisis.urutkanTermahalKeTermurah();
                    analisis.simpanHasilSorting("hasil_sort_penjualan.txt", "Termahal ke Termurah");
                } else if (jenisSorting == 2) {
                    analisis.urutkanTermurahKeTermahal();
                    analisis.simpanHasilSorting("hasil_sort_penjualan.txt", "Termurah ke Termahal");
                } else {
                    cout << "Pilihan tidak valid." << endl;
                }
                break;
            }
            
            case 0: {
                cout << "Terima kasih telah menggunakan program ini!" << endl;
                break;
            }
            
            default: {
                cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
                break;
            }
        }
        
        if (pilihan != 0) {
            cout << "\nTekan Enter untuk melanjutkan...";
            cin.ignore();
            cin.get();
        }
        
    } while (pilihan != 0);
    
    return 0;
}
