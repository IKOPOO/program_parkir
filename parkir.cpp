 /*
  judul : membuat sebuah sistem parkir kendaraan 
  oleh  : rico andre pratama
  waktu : semarang, 4 maret 2023 - bertahap 

  program ini menyakup 
  - waktu masuk kendaran masuk parkir -> clear
    -setiap kendaraan masuk mengambil waktu real time  -> clear
  - pengecekan daerah parkir mana yang kosong -> clear
  - pemberian tarif parkir berdasarkan jam -> clear
  - pengecekan data parkir -> clear
    - masuk jam berapa -> clear
    - keluar jam berapa -> clear
    - total waktu parkir -> clear
  -data parkir di cetak di database sebuah file txt -> TAHAP PERKEMBANGAN
    - contoh 
      - parkir no 20 
        - masuk parkir jam berapa 
        - keluar parkir jam berapa 
        - total parkir berapa jam 
        - berapa tarif parkir 
  
  fitur yang harus di tambahkan kedepannya 
  - pegawai
  - transaksi
  - parkir
  - tarif harga 

*/

/*
    alur program 
    - menu utama 
      - masuk
        - input data mobil
        - jenis mobil
        - pemilihan tempat parkir
      - keluar
        - input data mobil
        - melakukan transaksi
      - cek data parkir
        -input data mobil yang ingin di cek 
      - cek parkirkan kosong
        -output data parkir yang kosong
      - keluar program -> end program
*/

#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace std; 


//kamus global


//membuat tipe data buatan 
//cello
struct parkir{

  string noPlat;     //menyimpan no plat kendaraan 
  string jenisKendaraan;  //menyimpan jenis kendaraan yang masuk
  int noParkir;     //untuk menentukan no parkir kendaraan 
      
  //menyimpan waktu masuk dan keluar
  string waktuMasuk;  
  string waktuKeluar; 
  int durasi_parkir; 
  int totalharga;

  parkir *prev; 
  parkir *next; 

};
void single(){
  //kamus lokal 
  int x;
  //deskripsi 
  while(x < 60 )
  {
    cout << "-";
    x = x + 1 ;
  }
    cout << endl;
}

void garis(){
  //kamus lokal 
  int x;
  //deskripsi 
  while(x < 60 )
  {
    cout << "=";
    x = x + 1 ;
  }
    cout << endl;
}

void clearscreen(){
  system("cls");
}

//membuat fungsi untuk menampilkan waktu dan tanggal dan mengambil data waktu secara real time 
//rico andre pratama (A11.2023.15393)
string waktu(){
  //fungsi ini mengambil waktu saat ini dari sistem 
  auto sekarang = chrono::system_clock::now(); 
  //baris kode ini untuk mengkonversi time_point yang diambil sebelumnnya ke dalam bentuk time_t
  auto now_time_t = chrono::system_clock::to_time_t(sekarang);
  //mengkonversi time_t ke dalam std::tm yang merupakan pointer ke struc std::tm yang menyimpan struktur waktu
  //dalam format yang lebih terperinci 
  tm* time_info = localtime(&now_time_t); 

  //mengubah ke dalam format yang diingin kan 
  stringstream ss; 
  ss << put_time(time_info, "%H:%M:%S %d %B %Y");
  return ss.str();
}

//fungsi untuk mengkonversi string waktu ke dalam std::tm
tm stringToTm(const string& timeStr){
  tm tm = {}; 
  istringstream ss(timeStr); 
  ss >> get_time(&tm, "%H:%M:%S %d %B %Y"); 
  if(ss.fail()){
    cout << "gagal konversi " << endl; 
  }
  return tm; 
}

//fungsi untuk konversi std::tm ke std::chrono::system_clock::time_point
chrono::system_clock::time_point tmToTimePoint(const tm& tm){
  time_t timeT = mktime(const_cast<std::tm*>(&tm));
  return chrono::system_clock::from_time_t(timeT);
}

//fungsi mengurangi waktu 
int durasi(string waktumasuk, string waktukeluar){

  tm tm1 = stringToTm(waktumasuk); 
  tm tm2 = stringToTm(waktukeluar); 

  auto timepoint1 = tmToTimePoint(tm1); 
  auto timepoint2 = tmToTimePoint(tm2);

  auto duration = chrono::duration_cast<chrono::minutes>(timepoint2 - timepoint1); 
  return duration.count();

}


//fungsi untuk membuat node 
//atanasius marcello avta pendhi putra (A11.2023.15390)
parkir *head, *tail, *current, *afternode; 

parkir* buatnode(const parkir &data ){
  parkir *newnode = new parkir;
  newnode ->noPlat = data.noPlat; 
  newnode ->jenisKendaraan = data.jenisKendaraan; 
  newnode ->noParkir = data.noParkir; 
  newnode ->waktuMasuk = data.waktuMasuk; 
  newnode ->waktuKeluar = data.waktuKeluar; 
  newnode ->prev = nullptr; 
  newnode ->next = nullptr; 
  return newnode;  
}

 //jumlah parkir yang tersedia 
 //atanasius marcello avta pendhi putra (A11.2023.15390)
int batasmaksimum = 200;
//fungsi cek lokasi parkir sudah penuh atau belum
int hitung(){ // menghitung sudah berapa banyak data di dalam linked list
  int count = 0;
  current = head; 
  while (current != nullptr){
    
    count++; 
    current = current->next; 
  }

  return count;
  
}

//akbar dwi saputro (A11.2023.15371)
vector <int> urutP; // ini untuk menyimpan no parkir yang keluar
//prosedur update data setelah input node di tengah  
void update_noParkir(){ //memberikan no parkir kepada kendaraan yang masuk
  current = head; //kita mengupdate noParkir dari head untuk meminimalisir kesalahan saat update data nomor parkir
  int nomor = 1; //inisialisasi nomor parkir awal adalah  1 
  while(current != nullptr){ // jika current itu tidak nullpt maka perintah di bawah akan dijalankan 
    current ->noParkir = nomor; 
    current = current->next; 
    nomor++; 
  }
}

//fungsi untuk input awal
//akbar dwi saputro (A11.2023.15371)
void inputAwal(parkir &data){
  parkir *newnode = buatnode(data);
  newnode->next = head; 
  head ->prev = newnode; 
  head = newnode; 
}
//prosedur memasukkan data ke dalam node di akhir node 
//atanasius marcello avta pendhi putra (A11.2023.15390)
void inputAkhir(parkir &data){
  parkir* newnode = buatnode(data); 
  if(head == nullptr){
    head = newnode; //jika linked list masi kosong maka dia akan menjadi head
  }else{
    tail->next = newnode; // jika sudah tercapai node terakhir atau yang selanjut dari node tersebut itu nullptr maka membuat node baru
    newnode->prev = tail;
    tail = newnode; 
  }
}


int nomor = 2;
//prosedur untuk memasukkan data ke dalam linked list di awal 
// rico andre pratama (A11.2023.15393)
void masuk_parkir(parkir &data){ 

  
  if(hitung() >= batasmaksimum){ //jika parkiran penuh 
    cout << "Parkir sudah penuh, maaf :( \n"; 
    return; 
  }else{
    data.waktuMasuk = waktu();
    // current->waktuMasuk = waktu(); 

    //ini adalah kondisi jika list masih kosong/belum ada satu node yang dibuat 
    if(head == nullptr){ //
      
      data.noParkir = 1; 
      parkir *newnode = buatnode(data);
      head = newnode; 
      tail = newnode; 
      // nomor++;
    }else{ //ini adalah untuk memsukkan data parkir di tengah atau di akhir 

      current = head; 
      int temp = !urutP.empty() ? urutP.front() : nomor; //jika vector urutp tidak kosong maka akan mengambil 
      nomor++;
      parkir *newnode = buatnode(data);       //data indek 0 di vector, jika kosong maka akan mengambil isi dari variabel nomor
      int currentpos = 1; 
      while(currentpos < temp - 1 ){ 
        current = current->next; 
        currentpos++; 
      }
    
      if( current->next == nullptr){ //setelah memasukkan data pertama kali maka, memasukkan data kedua dan
              
        inputAkhir(data);            //seterusnya menggunakan fungsi ini 
        update_noParkir(); //untuk mengupdate no parkir kendaraan 
        // nomor++;
        
      }else{
        
        data.noParkir = urutP.front();  //jika sudah ada kendaraan yang keluar dan berada di tengah maka kondisi ini digunakan 
        afternode = current->next; 
        newnode-> prev = current;
        newnode->next = afternode;
        current->next = newnode;
        afternode->prev = newnode;  
        
        // urutP.erase(urutP.begin());
      }
    }
    // nomor++;
  }   
}

//atanasius marcello avta pendhi putra (A11.2023.15390)
void pembayaran(int totalharga){

  int totalpembayaran = 0; // variabel untuk menyimpan nominal pembayaran 
  
    do {
        cout << "Pembayaran saat ini " << setw(5) << ": ";
        int pembayaranSaatIni; //variabel untuk nominal pertama 
        cin >> pembayaranSaatIni; 
        cin.ignore();

        if (pembayaranSaatIni > 0) {
            totalpembayaran += pembayaranSaatIni;

            if (totalpembayaran < totalharga) {
                
              cout << "yahh uang kamu kurang nihh !!\n";
              cout << "uang kamu sekarang " << setw(6) << ": "<< totalpembayaran << endl;
              int sudahbayar = (totalharga - pembayaranSaatIni); 
              cout << "uang kamu kurang " << setw(8)  << ": " << sudahbayar << endl; 
              totalharga = sudahbayar; 
              sudahbayar = 0;
              pembayaranSaatIni = 0;
              totalpembayaran = 0; 
            }
        } else {
            cout << "Pembayaran harus lebih dari 0.\n";
        }

    } while (totalpembayaran < totalharga);
  
  cout << "kembalian " << setw(15) << ": " << totalpembayaran - totalharga << endl;
  cout << "transaksi berhasil :)\n";
  
}


//prosedur menampilkan data di dalam linked list 
//akbar dwi saputro (A11.2023.15371)
void tampildata(){
  current = head; 
  while(current != nullptr){
    cout << "no Plat          : " << current->noPlat << endl; 
    cout << "no parkir        : " << current ->noParkir << endl; 
    cout << "jenis kendaraan  : " << current->jenisKendaraan << endl; 
    cout << "waktu masuk      : " << current->waktuMasuk << endl; 
    cout << "waktu keluar     : " << current ->waktuKeluar << endl;  
    cout << endl;
    current = current->next; 
  }
}

//prosedur untuk menghapus data sesuai dengan kriteria 
//akbar dwi saputro (A11.2023.15371)
void hapus_data(const string &noPlat){
  current = head; 
  parkir *prev = nullptr; 
  parkir* temp; //dadta sementara untuk menyimpan data current
  while(current != nullptr){ //cek apakah data sekarang itu tidak nullptr -> jika tidak maka kode di eksekusi 
    if(current->noPlat == noPlat ){ //jika data no plat sekarang itu sesuai dengan kriterian maka balok kode akan di eksekusi 
      
      temp = current; //temp akan menyimpan data current atau data yang akan di hapus secara sementara
      // nomor = current->noParkir; 
      urutP.push_back(current->noParkir);
      if(prev != nullptr){ // untuk melihat apakah prev itu tidak null ptr atau iya 
        prev->next = current->next; // jika iya maka prev akan menunjuk ke data setelah data current yang akan di hapus 
      }else{  //jika prev nullptr
        head = current->next;   // maka prev berada di head dan dia harus menunjuk ke data selanjutnya atau membuah node baru
      }
    
      current = current -> next;  //setelah di hapus curren akan menunjuk data selanjutnya 
      delete temp; //data temp akan di hapus 
      break; //keluar dari loop karena data sudah berhasil di hapus 
    }else{
      prev = current; 
      current = current->next; 
    }
  }
}

//prosedur untuk mencari data yang sesuai dengan inputan 
// rico andre pratama (A11.2023.15393)
int cari_parkir(const string& noPlat, parkir &data){
  current = head; 
  bool ditemukan = false; 
  int harga = 10000;
  while(current != nullptr){
    if(current->noPlat == noPlat){

      //data ditemukan kemudian data di tampilkan 
      current->waktuKeluar = waktu(); //mengambil waktu keluar secara real time
      
      current->durasi_parkir = durasi(current->waktuMasuk,current->waktuKeluar);
      cout << "no Plat         : " << current->noPlat << endl; 
      cout << "no parkir       : " << current ->noParkir << endl; 
      cout << "jenis kendaraan : " << current->jenisKendaraan << endl; 
      cout << "waktu masuk     : " << current->waktuMasuk << endl; 
      cout << "waktu keluar    : " << current ->waktuKeluar << endl;  
      cout << "Durasi parkir   : " << current ->durasi_parkir << endl; 
      single();
      
      cout << "berikut harga yang harus anda bayar" << endl; 
      current->totalharga = ( harga * current->durasi_parkir);
      cout << "Total Harga : " << current->totalharga << endl; 
      pembayaran(current->totalharga);
      ditemukan = true; 
      break;
    }
    current = current->next; 
  }

  if(!ditemukan){ //jika ditemukan tetap false maka data tidak ada
    cout << "data tidak di temukan :_(" << endl; 
  }

  
  return ditemukan; 

}

  
//bersama-sama
main(){
  parkir data; //untuk menyimpan data parkir 
  
  while(true){

    string jawaban; 
    cout << "\t    Selamat Datang Di parkir Tembalang " << endl; 
    garis(); 
    cout << "Masuk \n" "keluar \n" "Tampil Data \n" "Cek Parkir Kosong \n" "Keluar Program\n";
    garis(); 
    cout << "Input : ";  
    getline(cin, jawaban);
      if(jawaban == "masuk"){
        
        clearscreen(); 
        cout << "Silahkan masukan data parkir :)\n";
        cout << "plat nomor" << "\t : "; 
        getline(cin, data.noPlat);
        cout << "Jenis kendaraan" << "\t : "; 
        getline(cin, data.jenisKendaraan);
        //memanggil prosedur input data
        masuk_parkir(data);
        single(); 
        string balasan;
        cout << "Ke-menu awal" << "\t : "; 
        getline (cin, balasan); 
        if(balasan == "iya"){          
          clearscreen(); 
        }

      } else if(jawaban == "keluar"){
        
          string cari;
          clearscreen(); 
          cout << "Plat kendaraan" << "\t : ";       
          getline(cin, cari);
          single();
          cari_parkir(cari,data);
          for(const int &n : urutP){
            cout << n << " "; 
          } 
          hapus_data(cari);
          single(); 
          string jawab; 
          cout << "Ke-menu awal" << "\t: ";
          getline(cin, jawab);
          if(jawab == "iya"){
            clearscreen();
          }

        }else if(jawaban == "tampil data"){
            
            clearscreen(); 
            tampildata(); 
            cout << endl; 
            string jawab; 
            cout << "ingin kembali ke menu utama ? \n" "iya/tidak : "; 
            getline(cin, jawab);
            if(jawab == "iya"){
              clearscreen(); 
            }

          }else if(jawaban == "keluar program"){
              break; 
            }else if(jawaban == "cek parkir"){
              if(urutP.empty() && hitung() >= batasmaksimum){
                cout << "Lahan parkir sudah penuh " << endl; 
                cout << "Tidak ada lokasi yang tersedia" << endl; 
              }else if(!urutP.empty() && hitung() < batasmaksimum){
                  cout << "lahan parkir yang tersedia : "; 
                  for(auto a : urutP){
                    cout << a << " ";  
                  }
                  cout << endl; 
                }else if(urutP.empty() && hitung() < batasmaksimum){
                    int lahan = batasmaksimum - hitung();
                    cout <<"Parkir yang tersedia " << lahan << endl; 
                  }else{
                    cout << "lahan parkir sudah penuh :(" << endl;
                  }
              string jawb; 
              single();
              cout << "Ke-menu utama : ";
              cin >> jawb; cin.ignore();
              if(jawb == "iya"){
                clearscreen();

              }
            } 
             else {
                
                cout << "perintah tidak valid :(" << endl ; 
                clearscreen(); 

              }           
  } 
}