#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

struct Produk {
    string kode;
    string nama;
    int jumlah;
    double harga;
    double totalPendapatan() const {
        return jumlah * harga;
    }
};

void bacaData(const string& namaFile, vector<Produk>& produkList) {
    ifstream file(namaFile);
    if (!file) {
        cerr << "Tidak dapat membuka file " << namaFile << endl;
        return;
    }

    Produk p;
    while (file >> p.kode >> p.nama >> p.jumlah >> p.harga) {
        produkList.push_back(p);
    }
    file.close();
}

void cariBarang(const vector<Produk>& produkList, const string& kode) {
    for (const auto& p : produkList) {
        if (p.kode == kode) {
            cout << "Ditemukan: " << p.nama << ", Jumlah: " << p.jumlah << ", Harga: " << p.harga << endl;
            return;
        }
    }
    cout << "Barang dengan kode " << kode << " tidak ditemukan." << endl;
}

void cariBarangTermurah(const vector<Produk>& produkList) {
    if (produkList.empty()) return;

    auto minIt = min_element(produkList.begin(), produkList.end(), [](const Produk& a, const Produk& b) {
        return a.harga < b.harga;
    });
    cout << "Barang termurah: " << minIt->nama << ", Harga: " << minIt->harga << endl;
}

void urutkanProduk(vector<Produk>& produkList, bool ascending) {
    sort(produkList.begin(), produkList.end(), [ascending](const Produk& a, const Produk& b) {
        return ascending ? a.harga < b.harga : a.harga > b.harga;
    });
}

double totalPendapatanPerProduk(const vector<Produk>& produkList) {
    double total = 0;
    for (const auto& p : produkList) {
        total += p.totalPendapatan();
    }
    return total;
}

void simpanHasilAnalisis(const string& namaFile, const vector<Produk>& produkList) {
    ofstream file(namaFile);
    if (!file) {
        cerr << "Tidak dapat membuka file " << namaFile << endl;
        return;
    }

    file << "Kode Produk, Nama Produk, Jumlah, Harga, Total Pendapatan\n";
    for (const auto& p : produkList) {
        file << p.kode << ", " << p.nama << ", " << p.jumlah << ", " << p.harga << ", " << p.totalPendapatan() << "\n";
    }
    file.close();
}

void simpanHasilSorting(const string& namaFile, const vector<Produk>& produkList) {
    ofstream file(namaFile);
    if (!file) {
        cerr << "Tidak dapat membuka file " << namaFile << endl;
        return;
    }

    for (const auto& p : produkList) {
        file << p.kode << ", " << p.nama << ", " << p.jumlah << ", " << p.harga << ", " << p.totalPendapatan() << "\n";
    }
    file.close();
}

int main() {
    vector<Produk> produkList;
    string namaFile = "data_penjualan.txt";

    bacaData(namaFile, produkList);

    string kodeCari;
    cout << "Masukkan kode barang yang dicari: ";
    cin >> kodeCari;
    cariBarang(produkList, kodeCari);

    cariBarangTermurah(produkList);

    urutkanProduk(produkList, false);
    cout << "Produk diurutkan dari termahal ke termurah:\n";
    for (const auto& p : produkList) {
        cout << p.nama << ": " << p.harga << endl;
    }

    urutkanProduk(produkList, true);
    cout << "Produk diurutkan dari termurah ke termahal:\n";
    for (const auto& p : produkList) {
        cout << p.nama << ": " << p.harga << endl;
    }

    double totalPendapatan = totalPendapatanPerProduk(produkList);
    cout << "Total pendapatan dari semua transaksi: " << totalPendapatan << endl;

    simpanHasilAnalisis("hasil_analisis_penjualan.txt", produkList);

    simpanHasilSorting("hasil_sort_penjualan.txt", produkList);

    return 0;
}

