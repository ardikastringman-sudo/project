#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <conio.h>   // Library Windows untuk _getch()
#include <windows.h> // Library Windows untuk kontrol konsol (Sleep, dll)

using namespace std;

// ==========================================
// 1. STRUKTUR DATA UTAMA
// ==========================================
enum Element { PHYSICAL, FIRE, ICE };

struct Skill {
    string name;
    Element element;
    int damage;
    int mpCost;
};

struct Entity {
    string name;
    int hp, maxHp;
    int mp, maxMp;
    Element weakness;
    vector<Skill> skills;
};

// Variabel Status Global
Entity player;
int karmaPoints = 0;
int socialLinkRank = 1;
bool gameRunning = true;

//  Function Pointer untuk State Machine 
// Menyimpan alamat fungsi stage yang sedang aktif
void (*currentStageFunc)() = nullptr;

// Deklarasi fungsi di awal agar bisa dipanggil oleh Function Pointer
void playStage1();
void playStage2();
void playEndingScene();

// ==========================================
// 2. FUNGSI UTILITAS 
// ==========================================
void clearScreen() {
    system("cls");
}

void printHeader(string title) {
    clearScreen();
    cout << "=========================================\n";
    cout << "  " << title << "\n";
    cout << "=========================================\n\n";
}

// ==========================================
// 3. SISTEM PERTARUNGAN 
// ==========================================
// POINTER 2: Pass-by-Pointer (Entity* enemy) 
void startBattle(Entity* enemy) {
    // Menggunakan -> karena enemy sekarang adalah alamat memori (pointer)
    printHeader("PERTEMPURAN DIMULAI: VS " + enemy->name);
    cout << "Analisis musuh: Cari kelemahannya menggunakan elemen!\n";
    cout << "Tekan tombol apa saja untuk bersiap...";
    _getch();

    while (player.hp > 0 && enemy->hp > 0) {
        printHeader("GILIRAN BERTARUNG");
        cout << enemy->name << " HP: " << enemy->hp << "\n";
        cout << player.name << " HP: " << player.hp << "/" << player.maxHp 
             << " | MP: " << player.mp << "/" << player.maxMp << "\n\n";
        
        cout << "Pilih Jurus:\n";
        for (size_t i = 0; i < player.skills.size(); ++i) {
            cout << i + 1 << ". " << player.skills[i].name 
                 << " (" << player.skills[i].mpCost << " MP)\n";
        }
        
        char choice = _getch(); 
        int idx = choice - '1';
        
        if (idx >= 0 && idx < (int)player.skills.size()) {
            Skill selected = player.skills[idx];
            if (player.mp >= selected.mpCost) {
                player.mp -= selected.mpCost;
                int dmg = selected.damage;
                
                if (selected.element == enemy->weakness) {
                    dmg *= 2;
                    cout << "\n[WEAKNESS!] " << enemy->name << " terkena serangan fatal!\n";
                    cout << "Kamu dapat bonus giliran menyerang lagi!\n";
                    enemy->hp -= dmg;
                    cout << "Damage masuk: " << dmg << "\n";
                    Sleep(1500); 
                    if (enemy->hp <= 0) break;
                    continue; 
                } else {
                    enemy->hp -= dmg;
                    cout << "\nKamu menyerang dengan " << selected.name << ". Damage: " << dmg << "\n";
                }
            } else {
                cout << "\nMP tidak cukup!\n";
            }
        }
        
        if (enemy->hp <= 0) break;

        // Giliran Musuh
        cout << "\n--- GILIRAN MUSUH ---\n";
        int enemyDmg = rand() % 15 + 5;
        player.hp -= enemyDmg;
        cout << enemy->name << " membalas seranganmu sebesar " << enemyDmg << " HP!\n";
        
        if (player.hp <= 0) {
            cout << "\n[GAME OVER] Anda telah gugur di medan perang.\n";
            gameRunning = false;
            return;
        }
        
        cout << "\nTekan tombol apa saja untuk lanjut ke giliran berikutnya...";
        _getch();
    }
    
    if (player.hp > 0) {
        cout << "\n[VICTORY] " << enemy->name << " berhasil dihancurkan!\n";
        cout << "Tekan tombol apa saja untuk melanjutkan cerita...";
        _getch();
    }
}

// ==========================================
// 4. KUMPULAN STAGE
// ==========================================
void playStage1() {
    printHeader("STAGE 1: KORIDOR KELAM");
    cout << "Anda terperangkap di labirin bawah tanah bersama seorang penyintas misterius.\n";
    cout << "Tiba-tiba, monster bayangan menghadang jalan keluar Anda.\n\n";
    cout << "Pilihan Keputusan:\n";
    cout << "[1] Lindungi penyintas dan maju menghadapi monster.\n";
    cout << "[2] Jadikan penyintas sebagai umpan untuk melarikan diri.\n";
    
    char choice = _getch();
    if (choice == '1') {
        karmaPoints += 10;
        socialLinkRank += 1;
        cout << "\n[Pilihan Dicatat] Anda memilih jalan kepahlawanan. Social Link meningkat.\n";
        
        // --- SISTEM MUSUH ACAK ---
        // 1. Buat Array berisi 3 jenis musuh dengan HP dan kelemahan berbeda
        Entity enemyPool[3] = {
            {"Shadow Slime", 60, 60, 0, 0, FIRE, {}},       // Lemah: Api
            {"Creeping Ghoul", 80, 80, 0, 0, ICE, {}},      // Lemah: Es
            {"Cursed Armor", 100, 100, 0, 0, PHYSICAL, {}}  // Lemah: Fisik (Keras tapi lemah pukulan)
        };
        
        // 2. Hasilkan angka acak antara 0 sampai 2
        int randomIndex = rand() % 3;
        
        cout << "\n[WASPADA] Seekor " << enemyPool[randomIndex].name << " muncul dari kegelapan!\n";
        Sleep(1500); // Jeda sejenak agar pemain bisa membaca nama musuh
        
        // 3. Mulai pertarungan menggunakan musuh yang terpilih secara acak
        // Gunakan '&' untuk mengambil alamat memori dari elemen array tersebut
        startBattle(&enemyPool[randomIndex]); 
        // -------------------------
        
        // Mengarahkan pointer fungsi ke stage berikutnya
        if (gameRunning) currentStageFunc = playStage2; 
    } else {
        karmaPoints -= 15;
        cout << "\n[Pilihan Dicatat] Anda mengorbankan rekan demi keselamatan sendiri...\n";
        cout << "Anda lolos dari area ini tanpa bertarung, namun udara terasa semakin dingin.\n";
        _getch();
        currentStageFunc = playStage2; // Mengarahkan pointer fungsi
    }
}
void playStage2() {
    printHeader("STAGE 2: RUANG KACA");
    cout << "Anda tiba di ruangan yang dipenuhi cermin retak.\n";
    cout << "Penyintas misterius tadi menatap pantulan dirinya dengan takut.\n";
    cout << "Monster penjaga gerbang utama muncul menghalangi jalan.\n\n";
    
    if (socialLinkRank > 1) {
        cout << "Rekan Anda memberikan ramuan obat, HP dan MP Anda pulih sepenuhnya!\n\n";
        player.hp = player.maxHp;
        player.mp = player.maxMp;
    } else {
        cout << "Efek rasa bersalah membuat fokus Anda menurun (Maksimal HP berkurang 10).\n\n";
        player.hp -= 10;
    }
    
    cout << "Tekan tombol apa saja untuk menghadapi Boss Gerbang...";
    _getch();
    
    // POINTER 3: Alokasi Dinamis menggunakan Keyword 'new' 
    Entity* gateKeeper = new Entity{"Gatekeeper Minotaur", 100, 100, 0, 0, ICE, {}};
    
    // Parameter sudah berupa pointer, tidak perlu '&' lagi
    startBattle(gateKeeper); 
    
    // Wajib menghapus memori setelah pointer selesai digunakan
    delete gateKeeper; 
    
    if (gameRunning) currentStageFunc = playEndingScene; 
}

void playEndingScene() {
    printHeader("EPILOG: GERBANG REALITAS");
    cout << "Pertualangan Anda berakhir di sini. Gerbang dimensi mulai tertutup.\n\n";
    
    if (karmaPoints >= 10) {
        cout << "[GOOD ENDING: Harapan Baru]\n";
        cout << "Anda berhasil keluar bersama rekan Anda. Hubungan kuat yang terjalin\n";
        cout << "menjadi tameng emosional yang menyembuhkan trauma dunia bayangan.\n";
    } else if (karmaPoints < 0 && karmaPoints > -15) {
        cout << "[NORMAL ENDING: Sendiri dalam Sepi]\n";
        cout << "Anda selamat dan kembali ke dunia nyata. Namun, Anda menghabiskan\n";
        cout << "sisa hidup dalam kesunyian karena ingatan akan rekan yang Anda tinggalkan.\n";
    } else {
        cout << "[BAD ENDING: Terperangkap Selamanya]\n";
        cout << "Karma buruk mengikat kaki Anda. Saat Anda mencoba melompat ke gerbang,\n";
        cout << "bayangan dari dosa masa lalu menarik Anda jatuh ke dalam kegelapan abadi.\n";
    }
    
    gameRunning = false; 
}

// ==========================================
// 5. LOOP UTAMA GAME
// ==========================================
int main() {
    srand(time(0));
    cout << "Masukkan nama karakter Anda: ";
    cin >> player.name;
    player.hp = player.maxHp = 100;
    player.mp = player.maxMp = 40;
    player.skills = {
        {"Serangan Fisik", PHYSICAL, 12, 0},
        {"Sihir Api (Pernapasan Api)", FIRE, 20, 8},
        {"Sihir Es (Ice Shard)", ICE, 20, 8}
    };

    // Tentukan stage awal menggunakan pointer
    currentStageFunc = playStage1;

    // Loop game yang jauh lebih elegan dan ringan
    while (gameRunning && currentStageFunc != nullptr) {
        currentStageFunc(); // Mengeksekusi fungsi yang sedang ditunjuk oleh pointer
    }

    cout << "\n=========================================\n";
    cout << "       GAME OVER - TERIMA KASIH\n";
    cout << "========================================\n";
    return 0;
}