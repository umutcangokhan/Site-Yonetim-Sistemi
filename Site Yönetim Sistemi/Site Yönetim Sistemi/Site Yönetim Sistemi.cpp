#include <iostream>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <cctype>
#include <string>
#include <conio.h>

using namespace std;

#define MAX_DAIRE 100 // Maksimum daire sayısı
#define AIDAT 1000 // aidat değeri.

double sayialma(bool ondalikIzin = true) {  //İnputları getch fonksiyonu aracılığı ile string dizisi şeklinde alma fonksiyonu
    char a;
    string stringdizisi = "";
    bool noktaKullanildi = false;

    do {



        a = _getch(); // Karakteri oku ve ekrana yaz

        if (isdigit(a)) { // Eğer rakam ise ekrana yazdır
            stringdizisi += a;
            cout << a;
        }
        else if (a == '.' && ondalikIzin && !noktaKullanildi) { // Double değeri girilecekse ve henuz nokta konulmadıysa
            stringdizisi += a;
            cout << a;
            noktaKullanildi = true;
        }
        else if (a == '\b' && !stringdizisi.empty()) { // Backspace tuşuna basılmışsa ve string dizisi boş değilse 
            cout << "\b \b"; // Karakteri ekrandan sil
            if (stringdizisi.back() == '.') { // Eğer son karakter nokta ise
                noktaKullanildi = false; // Nokta kullanımını serbest bırak
            }
            stringdizisi.pop_back(); // Diziden son karakteri sil
        }
    } while (a != '\r'); // Enter tuşuna basılana kadar döngü devam eder

    return stringdizisi.empty() ? sayialma() : stod(stringdizisi);
}

fstream dosya;

class Oturanlar {                                           //OTURANLAR SINIFI
public:
    string ad;

    Oturanlar(const string& ad) : ad(ad) {}

    virtual ~Oturanlar() = default;
};

class DaireSahibi : public Oturanlar {                      //DAİRESAHİBİ ALT SINIFI
public:
    DaireSahibi(const string& ad) : Oturanlar(ad) {}

    void adDegistir(const string& yeniAd) {
        ad = yeniAd;
    }
};

class Misafir : public Oturanlar {                        // MİSAFİR ALT SINIFI
public:
    Misafir(const string& ad) : Oturanlar(ad) {}
};

class Mekan {                                              // MEKAN SINIFI
public:
    class Daire {                                  //DAİRE ALT SINIFI
    public:
        int daireNo;
        DaireSahibi* sahibi;
        Misafir* misafirler[5];
        int misafirSayisi = 0;
        double borc;

        Daire(int no, DaireSahibi* sahibi) : daireNo(no), sahibi(sahibi), borc(AIDAT) {     // KURUCU FONK
            for (int i = 0; i < 5; i++) misafirler[i] = nullptr;
        }

        ~Daire() {
            delete sahibi;
            for (int i = 0; i < misafirSayisi; i++) delete misafirler[i];
        }

        void misafirEkle() {                                         //   FONKSİYONLAR
            if (misafirSayisi < 5) {
                cout << "\nMisafirin adını giriniz: ";
                string misafirAd;
                cin >> misafirAd;
                Misafir* misafir = new Misafir(misafirAd);
                misafirler[misafirSayisi++] = misafir;
                cout << misafir->ad << " misafir olarak eklendi." << endl;
            }
            else {
                cout << "Kapasite dolmuş." << endl;
            }
            return;
        }

        void misafirSil() {
            cout << "\nSilmek istediğiniz misafirin adını giriniz: ";
            string misafirAdi;
            cin >> misafirAdi;

            for (int i = 0; i < misafirSayisi; i++) {
                if (misafirler[i]->ad == misafirAdi) {
                    delete misafirler[i];
                    for (int j = i; j < misafirSayisi - 1; j++) {
                        misafirler[j] = misafirler[j + 1];
                    }
                    misafirler[--misafirSayisi] = nullptr;
                    cout << misafirAdi << " isimli misafir silindi." << endl;
                    return;
                }
            }
            cout << misafirAdi << " isimli misafir bulunamadı." << endl;
        }

        void daireSahibiDegistir() {
            cout << "\nYeni daire sahibinin adını giriniz: ";
            string yeniAd;
            cin >> yeniAd;
            sahibi->adDegistir(yeniAd);
            system("cls");
            cout << "Daire sahibi adı " << yeniAd << " olarak değiştirildi." << endl;
        }

    };

    Daire* daireler[MAX_DAIRE];        // DAİRE NESNESİ
    int daireSayisi = 0;

    class Havuz : public Daire {         // HAVUZ ALT SINIFI
    public:
        bool havuzKullanabilir;
    };

    void havuzKullan() {                     // HAVUZ KULLANIM FONKSİYONU
        cout << "Havuzu kullanacak dairenin numarasını giriniz: ";
        int numara;
        numara = sayialma(false);
        for (int i = 0; i < daireSayisi; i++) {

            if (daireler[i]->daireNo == numara) {
                if (daireler[i]->borc > 0) {
                    cout << "Bu dairenin " << daireler[i]->borc << " TL borcu bulunduğu için havuz kullanımı gerçekleşemedi." << endl;
                    return;
                }

                cout << "Daire " << numara << " sakinleri: " << daireler[i]->sahibi->ad;
                for (int j = 0; j < daireler[i]->misafirSayisi; j++) {
                    cout << " " << daireler[i]->misafirler[j]->ad;
                }
                cout << endl << "Havuzu kullanacak kişiyi seçiniz: ";
                string havuzKullanıcı;
                cin >> havuzKullanıcı;

                if (havuzKullanıcı == daireler[i]->sahibi->ad) {
                    dosya.open("havuzkullanımı.txt", ios::app);
                    dosya << "Daire " << numara << " " << daireler[i]->sahibi->ad << " havuzu kullandı." << endl;
                    dosya.close();
                    cout << "Havuz kullanımı başarıyla kaydedildi." << endl;
                    return;
                }

                for (int k = 0; k < daireler[i]->misafirSayisi;k++) {
                    if (havuzKullanıcı == daireler[i]->misafirler[k]->ad) {
                        dosya.open("havuzkullanımı.txt", ios::app);
                        dosya << "Daire " << numara << " " << daireler[i]->misafirler[k]->ad << " havuzu kullandı." << endl;
                        dosya.close();
                        cout << "Havuz kullanımı başarıyla kaydedildi." << endl;
                        return;
                    }
                }
                cout << "Böyle bir kullanıcı yok." << endl;
                return;
            }
        }
        cout << "Böyle bir daire yok." << endl;
        return;
    }

    void fitnessKullan() {           // FİTNESS KULLANIM FONKSİYONU
        cout << "Fitness'ı kullanacak dairenin numarasını giriniz: ";
        int numara;
        numara = sayialma(false);
        for (int i = 0; i < daireSayisi; i++) {

            if (daireler[i]->daireNo == numara) {
                if (daireler[i]->borc > 0) {
                    cout << "Bu dairenin " << daireler[i]->borc << " TL borcu bulunduğu için fitness kullanımı gerçekleşemedi." << endl;
                    return;
                }

                cout << "Daire " << numara << " sakinleri: " << daireler[i]->sahibi->ad;
                for (int j = 0; j < daireler[i]->misafirSayisi; j++) {
                    cout << " " << daireler[i]->misafirler[j]->ad;
                }
                cout << endl << "Fitness'ı kullanacak kişiyi seçiniz: ";
                string fitnessKullanıcı;
                cin >> fitnessKullanıcı;

                if (fitnessKullanıcı == daireler[i]->sahibi->ad) {
                    dosya.open("fitnesskullanımı.txt", ios::app);
                    dosya << "Daire " << numara << " " << daireler[i]->sahibi->ad << " fitness'ı kullandı." << endl;
                    dosya.close();
                    cout << "Fitness kullanımı başarıyla kaydedildi." << endl;
                    return;
                }

                for (int k = 0; k < daireler[i]->misafirSayisi;k++) {
                    if (fitnessKullanıcı == daireler[i]->misafirler[k]->ad) {
                        dosya.open("fitnesskullanımı.txt", ios::app);
                        dosya << "Daire " << numara << " " << daireler[i]->misafirler[k]->ad << " fitness'ı kullandı." << endl;
                        dosya.close();
                        cout << "Fitness kullanımı başarıyla kaydedildi." << endl;
                        return;
                    }
                }
                cout << "Böyle bir kullanıcı yok." << endl;
                return;
            }
        }
        cout << "Böyle bir daire yok." << endl;
        return;
    }

    void daireEkle() {
        if (daireSayisi < MAX_DAIRE) {
            cout << "Daire numarasını giriniz: ";
            int numara;
            numara = sayialma(false);
            cout << "\nDaire sahibi adını giriniz: ";
            string sahibiAd;
            cin >> sahibiAd;
            DaireSahibi* yeniSahibi = new DaireSahibi(sahibiAd);
            daireler[daireSayisi++] = new Daire(numara, yeniSahibi);
            cout << "Daire " << numara << " başarıyla eklendi.\n" << endl;
        }
        else {
            system("cls");
            cout << "Daire kapasitesi dolmuş.\n" << endl;
        }
        return;
    }

    void daireSil() {
        if (daireSayisi == 0) {
            system("cls");
            cout << "Henüz kayıtlı daire bulunmamaktadır.\n\n";
            return;
        }
        cout << "Silmek istediğiniz dairenin numarasını giriniz: ";
        int numara;
        numara = sayialma(false);

        for (int i = 0; i < daireSayisi; i++) {
            if (daireler[i]->daireNo == numara) {
                delete daireler[i];
                for (int j = i; j < daireSayisi - 1; j++) {
                    daireler[j] = daireler[j + 1];
                }
                daireler[--daireSayisi] = nullptr;
                cout << "\nDaire no " << numara << " silindi.\n" << endl;
                return;
            }
        }
        cout << "\nDaire bulunamadı.\n" << endl;
    }

    void odemeYap(int numara) {
        for (int i = 0; i < daireSayisi; i++) {
            if (daireler[i]->daireNo == numara) {
                cout << "\nDaire " << numara << " " << daireler[i]->borc << " TL borca sahip. Ödemek istediğiniz tutarı giriniz: ";
                int odenenTutar;
                do {
                    odenenTutar = sayialma(true);
                    if (odenenTutar > daireler[i]->borc) cout << "Borcunuzdan fazlasını ödeyemezsiniz lütfen tekrar giriniz: ";
                    else if (odenenTutar <= 0) cout << "Geçersiz bir değer girdiniz lütfen tekrar giriniz: ";
                    else {
                        dosya.open("ödemeler.txt", ios::app);
                        daireler[i]->borc -= odenenTutar;
                        dosya << "Daire " << numara << "'dan " << odenenTutar << "TL başarıyla ödendi.\n" << "Kalan borç: " << daireler[i]->borc << endl << endl;
                        dosya.close();
                        return;
                    }
                } while (!0 < odenenTutar <= daireler[i]->borc);
            }
            system("cls");
            cout << "Daire bulunamadı." << endl;
        }
    }

    void yazdir() {
        dosya.open("data.txt");

        for (int i = 0; i < daireSayisi; i++) {
            dosya << "Daire Numarası: " << daireler[i]->daireNo << endl
                << "Daire Sahibi: " << daireler[i]->sahibi->ad << endl
                << "Misafirler :";

            for (int j = 0; j < daireler[i]->misafirSayisi;j++) {
                cout << daireler[i]->misafirler[j]->ad << " ";
            };
            cout << endl;
        }
        dosya.close();

        dosya.open("mekan.txt");
        dosya << "Havuz\nFitness\n";
        for (int i = 0; i < daireSayisi; i++) {
            dosya << "Daire " << daireler[i]->daireNo << endl;
        }
        cout << "Listeleme Başaralı." << endl;
        return;
    }
};

int main() {
    setlocale(LC_ALL, "Turkish");

    Mekan mekan;
    int secim;

    do {       // MENU
        cout << "1. Daire Ekle\n2. Daire Sil\n3. Misafir Ekle\n4. Misafir Sil\n5. Daire sahibi degistir\n6. Listeleme\n7. Ödeme yap\n8. Havuz kullanımı\n9. Fitness kullanımı\n10. Çıkış\nSeçiminizi yapınız: ";
        secim = sayialma(false);

        switch (secim) {
        case 1:
            system("cls");
            mekan.daireEkle();
            break;
        case 2:
            if (mekan.daireSayisi > 0) {
                system("cls");
                mekan.daireSil();
            }
            else {
                system("cls");
                cout << "Seçim: Daire silme\nHenüz bir daire eklenmedi!\n" << endl;
            }

            break;
        case 3:
            if (mekan.daireSayisi > 0) {
                system("cls");
                cout << "Misafir eklemek istediğiniz dairenin numarasını giriniz: ";
                int numara;
                numara = sayialma(false);
                for (int i = 0; i < mekan.daireSayisi; i++) {
                    if (mekan.daireler[i]->daireNo == numara) {
                        mekan.daireler[i]->misafirEkle();
                        break;
                    }
                }
            }
            else {
                system("cls");
                cout << "Seçim: Misafir ekleme\nHenüz bir daire eklenmedi!\n" << endl;
            }

            break;
        case 4:
            if (mekan.daireSayisi > 0) {
                system("cls");
                cout << "Misafir silmek istediğiniz dairenin numarasını giriniz: ";
                int numara;
                numara = sayialma(false);
                for (int i = 0; i < mekan.daireSayisi; i++) {
                    if (mekan.daireler[i]->daireNo == numara) {
                        mekan.daireler[i]->misafirSil();
                        break;
                    }
                }
                cout << "Böyle bir daire yok.\n" << endl;
            }
            else {
                system("cls");
                cout << "Seçim: Misafir silme\nHenüz bir daire eklenmedi!\n" << endl;
            }
            break;
        case 5:
            if (mekan.daireSayisi > 0) {
                system("cls");
                cout << "Sahibinin adını değiştirmek istediğiniz dairenin numarasını giriniz: ";
                int numara;
                numara = sayialma(false);
                for (int i = 0; i < mekan.daireSayisi; i++) {
                    if (mekan.daireler[i]->daireNo == numara) {
                        mekan.daireler[i]->daireSahibiDegistir();
                        break;
                    }
                }
            }
            else {
                system("cls");
                cout << "Seçim: Daire sahibi adı değiştirme\nHenüz bir daire eklenmedi!\n" << endl;
            }
            break;
        case 6:
            if (mekan.daireSayisi > 0) {
                system("cls");
                mekan.yazdir();
            }
            else {
                system("cls");
                cout << "Seçim: Listeleme\nHenüz bir daire eklenmedi!\n" << endl;
            }
            break;
        case 7:
            if (mekan.daireSayisi > 0) {
                system("cls");
                cout << "Ödeme yapmak istediğiniz dairenin numarasını giriniz: ";
                int numara;
                numara = sayialma(false);
                mekan.odemeYap(numara);
            }
            else {
                system("cls");
                cout << "Seçim: Ödeme yapma\nHenüz bir daire eklenmedi!\n" << endl;
            }
            break;
        case 8:
            if (mekan.daireSayisi > 0) {
                system("cls");
                mekan.havuzKullan();
            }
            else {
                system("cls");
                cout << "Secim: Havuz kullanımı\nHeniz bir daire eklenmedi!\n" << endl;
            }
            break;
        case 9:
            if (mekan.daireSayisi > 0) {
                system("cls");
                mekan.fitnessKullan();
            }
            else {
                system("cls");
                cout << "Secim: Fitness kullanımı\nHeniz bir daire eklenmedi!\n" << endl;
            }
            break;

        case 10:
            cout << "\nÇıkış yapılıyor..." << endl;
            break;
        default:
            system("cls");
            cout << "\nGeçersiz seçim!\n" << endl;
        }
    } while (secim != 10);

    return 0;
}