#include <stdio.h>

// Peynir sayısı ve aile sayısı ilerde değiştirilirse tek bir noktadan değiştirilebilir
#define PEYNIR_SAYISI 8
#define AILE_SAYISI 10

// nesne odaklı yaklaşım olmasa iki boyutlu dizi(matris) kullanılabilirdi ama böyle veri ilişkisi korunmuş ve okunabilirlik artmış oldu.
// unsigned char veri tipiyle optimum alan kullanımı sağlanmış oldu.
struct Peynir
{
    unsigned char aileler[AILE_SAYISI];
    unsigned char toplam;
};
unsigned char enIyiPeynirBul(struct Peynir *peynirler, unsigned char peynirSayisi);
unsigned char enAzBegenenAileBul(struct Peynir *peynirler, unsigned char peynirSayisi, unsigned char aileSayisi);

int main()
{
    FILE *pDosya, *sDosya;                                      // dosyalara erişmek için işaretçi tanımlaması yapılır.
    char pDosyaAd[30], sDosyaAd[30];

    struct Peynir peynirler[PEYNIR_SAYISI];
    struct Peynir *pPeynirler = peynirler;

    // Dosyayı aç
    while (1)
    {
        printf("Puan dosyasinin adini giriniz: ");
        scanf("%29s", pDosyaAd);                                // taşmayı önlemek amacıyla 29s kullanılmıştır.
        pDosya = fopen(pDosyaAd, "r");                          // dosyayı okuma modunda açar.
        if (pDosya == NULL)
        {
            printf("Puan dosyasi acilamadi! Tekrar deneyiniz.\n");
            continue;                                           // Doğru txt ismi girinceye kadar döngü devam eder
        }
        else
            break;
    }

    // Dosyadan puanları oku
    for (int i = 0; i < PEYNIR_SAYISI; i++)
    {
        pPeynirler[i].toplam = 0;

        for (int j = 0; j < AILE_SAYISI; j++)
        {
            int tmp; // fscanf int okur

            if (fscanf(pDosya, "%d", &tmp) != 1)
            {                                                   // okunan değer int ise fscanf fonksiyonu '1' değeri döndürür
                printf("Dosya format hatasi!\n");
                fclose(pDosya);
                system("pause");
                return 1;
            }

            if (tmp >= 0 && tmp <= 4)
            {                                                  // girilen degerlerin uygun aralıkta olup olmadığı kontrol edilir (0-4 arasında ikisi de dahil)
                pPeynirler[i].aileler[j] = (unsigned char)tmp; // girilen degerler uygun aralıktaysa int veri tipi chara dönüştürülerek yapıya eklenir
                pPeynirler[i].toplam += tmp;                   // girile deger uygun indexe göre sırlama için toplam degeri de kaydedilir
            }
            else
            {
                printf(
                    "Yalnizca 0-4 arasinda puan girilebilir. "
                    "%d. peynir, %d. aile gecersiz puan vermistir: %d\n",
                    i + 1, j + 1, tmp);
                fclose(pDosya);
                system("pause");
                return 1;
            }
        }
    }
    fclose(pDosya);                                             // dosyayı kapatma

    // enIyiPeynirBul ve enAzBegenenAileBul fonksiyonları çağrıldı
    unsigned char enIyiPeynirIx = enIyiPeynirBul(peynirler, PEYNIR_SAYISI);
    unsigned char enAzAileIx = enAzBegenenAileBul(peynirler, PEYNIR_SAYISI, AILE_SAYISI);

    // Sonuç dosyasına yaz
    printf("Sonuc dosyasinin adini giriniz: ");
    scanf("%29s", sDosyaAd);
    sDosya = fopen(sDosyaAd, "w");                              // dosyayı yazma modunda açar
    if (sDosya == NULL)
    {
        printf("Hata: Sonuc dosyasi olusturulamadi veya yazma izni yok!\n");
        return 1;                                               // Programı hata ile sonlandır
    }

    // fprintf fonksiyonunun dönen değerini kontrol edebiliriz
    // fprintf başarılıysa yazılan karakter sayısını, başarısızsa negatif bir değer döner
    if (fprintf(sDosya, "En fazla begenilen peynir: %d\n", enIyiPeynirIx + 1) < 0 ||
        fprintf(sDosya, "Urunleri en az begenen aile: %d\n", enAzAileIx + 1) < 0)
    {

        printf("Hata: Dosyaya yazma sirasinda bir sorun olustu!\n");
        fclose(sDosya);
        return 1;
    }

    fclose(sDosya);
    printf("Sonuclar '%s' dosyasina basariyla yazildi.\n", sDosyaAd);
    return 0;
}

unsigned char enIyiPeynirBul(struct Peynir *peynirler, unsigned char peynirSayisi)
{
    // En fazla beğenilen peynir
    // 2. bir kontrol yapısı kullanarak en yüksek aynı puana sahip olan peynir varsa tespit edilebilir ancak soruda belirtilmediği için gerek duyulmamıştır.
    unsigned char enIyiPeynirIndex = 0; // Başlangıçta 0.peyniri en iyi peynir olarak kabul ediyoruz.
    for (int i = 1; i < peynirSayisi; i++)
    {
        if (peynirler[i].toplam > peynirler[enIyiPeynirIndex].toplam)
        {
            enIyiPeynirIndex = i;
        }
    }
    return enIyiPeynirIndex;
}

unsigned char enAzBegenenAileBul(struct Peynir *peynirler, unsigned char peynirSayisi, unsigned char aileSayisi)
{
    // En az beğenen aile
    unsigned char minToplam = 0;
    unsigned char enAzBegenenAileIndex = 0;

    for (int i = 0; i < peynirSayisi; i++)
        minToplam += peynirler[i].aileler[0];        // ilk ailenin toplam degeri minumum olarak alınır kıyaslama olarak bu alınacak böylelikle belirli değerlere bağlı kalınmaz

    for (int j = 1; j < aileSayisi; j++)
    {
        unsigned char aileToplam = 0;

        for (int i = 0; i < peynirSayisi; i++)
        {
            aileToplam += peynirler[i].aileler[j];
        }
        if (aileToplam < minToplam)
        {
            minToplam = aileToplam;
            enAzBegenenAileIndex = j;
        }
    }
    return enAzBegenenAileIndex;
}
