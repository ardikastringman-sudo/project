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
string currentStage = "STAGE_1"; // Mengontrol stage aktif

// ==========================================
// 2. FUNGSI UTILITAS (SISTEM WINDOWS)
// ==========================================
void clearScreen() {
    // Fungsi bawaan Windows untuk membersihkan layar dengan cepat
    system("cls");
}

void printHeader(string title) {
    clearScreen();
    cout << "=========================================\n";
    cout << "  " << title << "\n";
    cout << "=========================================\n\n";
}

// ==========================================
// 3. SISTEM PERTARUNGAN (BATTLE SYSTEM)
// ==========================================
void startBattle(Entity enemy) {
    printHeader("PERTEMPURAN DIMULAI: VS " + enemy.name);
    cout << "Analisis musuh: Cari kelemahannya menggunakan elemen!\n";
    cout << "Tekan tombol apa saja untuk bersiap...";
    _getch();

    while (player.hp > 0 && enemy.hp > 0) {
        printHeader("GILIRAN BERTARUNG");
        cout << enemy.name << " HP: " << enemy.hp << "\n";
        cout << player.name << " HP: " << player.hp << "/" << player.maxHp 
             << " | MP: " << player.mp << "/" << player.maxMp << "\n\n";
        
        cout << "Pilih Jurus:\n";
        for (size_t i = 0; i < player.skills.size(); ++i) {
            cout << i + 1 << ". " << player.skills[i].name 
                 << " (" << player.skills[i].mpCost << " MP)\n";
        }
        
        char choice = _getch(); // Fungsi Windows asli, langsung membaca input
        int idx = choice - '1';
        
        if (idx >= 0 && idx < (int)player.skills.size()) {
            Skill selected = player.skills[idx];
            if (player.mp >= selected.mpCost) {
                player.mp -= selected.mpCost;
                int dmg = selected.damage;
                
                if (selected.element == enemy.weakness) {
                    dmg *= 2;
                    cout << "\n[WEAKNESS!] " << enemy.name << " terkena serangan fatal!\n";
                    cout << "Kamu dapat bonus giliran menyerang lagi!\n";
                    enemy.hp -= dmg;
                    cout << "Damage masuk: " << dmg << "\n";
                    Sleep(1500); // Delay Windows (1.5 detik) agar pemain sempat membaca
                    if (enemy.hp <= 0) break;
                    continue; 
                } else {
                    enemy.hp -= dmg;
                    cout << "\nKamu menyerang dengan " << selected.name << ". Damage: " << dmg << "\n";
                }
            } else {
                cout << "\nMP tidak cukup!\n";
            }
        }
        
        if (enemy.hp <= 0) break;

        // Giliran Musuh
        cout << "\n--- GILIRAN MUSUH ---\n";
        int enemyDmg = rand() % 15 + 5;
        player.hp -= enemyDmg;
        cout << enemy.name << " membalas seranganmu sebesar " << enemyDmg << " HP!\n";
        
        if (player.hp <= 0) {
            cout << "\n[GAME OVER] Anda telah gugur di medan perang.\n";
            gameRunning = false;
            return;
        }
        
        cout << "\nTekan tombol apa saja untuk lanjut ke giliran berikutnya...";
        _getch();
    }
    
    if (player.hp > 0) {
        cout << "\n[VICTORY] " << enemy.name << " berhasil dihancurkan!\n";
        cout << "Tekan tombol apa saja untuk melanjutkan cerita...";
        _getch();
    }
}

// ==========================================
// 4. KUMPULAN STAGE (PONDASI MODULAR)
// ==========================================

// --- CARA TAMBAH STAGE BARU ---
// Cukup buat fungsi void baru di bawah ini, lalu daftarkan namanya di fungsi main().

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
        
        // Setup musuh untuk Stage 1
        Entity shadowSlime = {"Shadow Slime", 60, 60, 0, 0, FIRE, {}}; // Lemah terhadap api
        startBattle(shadowSlime);
        
        if (gameRunning) currentStage = "STAGE_2"; // Pindah ke stage berikutnya jika menang
    } else {
        karmaPoints -= 15;
        cout << "\n[Pilihan Dicatat] Anda mengorbankan rekan demi keselamatan sendiri...\n";
        cout << "Anda lolos dari area ini tanpa bertarung, namun udara terasa semakin dingin.\n";
        _getch();
        currentStage = "STAGE_2"; 
    }
}

void playStage2() {
    printHeader("STAGE 2: RUANG KACA");
    cout << "Anda tiba di ruangan yang dipenuhi cermin retak.\n";
    cout << "Penyintas misterius tadi (jika masih hidup) menatap pantulan dirinya dengan takut.\n";
    cout << "Monster penjaga gerbang utama muncul menghalangi jalan.\n\n";
    
    // Cerita berubah dinamis tergantung keputusan di stage sebelumnya
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
    
    Entity gateKeeper = {"Gatekeeper Minotaur", 100, 100, 0, 0, ICE, {}}; // Lemah terhadap es
    startBattle(gateKeeper);
    
    if (gameRunning) currentStage = "ENDING_SCENE"; // Menuju konklusi cerita
}

void playEndingScene() {
    printHeader("EPILOG: GERBANG REALITAS");
    cout << "Pertualangan Anda berakhir di sini. Gerbang dimensi mulai tertutup.\n\n";
    
    // Sistem Ending bercabang multi-kondisi mirip Little Nightmares
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
    
    gameRunning = false; // Menghentikan loop utama game
}

// ==========================================
// 5. LOOP UTAMA GAME (STATE MACHINE)
// ==========================================
int main() {
    // Inisialisasi awal acak dan status pemain
    srand(time(0));
    player = {"Yuu", 100, 100, 40, 40, PHYSICAL, {
        {"Serangan Fisik", PHYSICAL, 12, 0},
        {"Sihir Api (Agi)", FIRE, 20, 8},
        {"Sihir Es (Bufu)", ICE, 20, 8}
    }};

    // Game Loop yang mengatur jalannya stage secara dinamis
    while (gameRunning) {
        if (currentStage == "STAGE_1") {
            playStage1();
        } 
        else if (currentStage == "STAGE_2") {
            playStage2();
        } 
        else if (currentStage == "ENDING_SCENE") {
            playEndingScene();
        }
    }

    cout << "\n=========================================\n";
    cout << "       GAME OVER - TERIMA KASIH\n";
    cout << "=========================================\n";
    return 0;
}
