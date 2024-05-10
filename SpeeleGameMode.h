#pragma once // domāts lai novērstu liekas atsauces citos failos
#include "UserInterface/StartScreenWidget.h"// Start screen paneļa atsauce
#include "CoreMinimal.h" // nodrošina, ka ir iekļauta tikai minimālā pamata funkcionalitāte, kas var palīdzēt samazināt kompilācijas laiku
#include "GameFramework/GameModeBase.h" // atsauce kas ļauj izmantot GameModeBase funkcionalitāti
#include "SpeeleGameMode.generated.h" // ietver ģenerētu C++ kodu, kas nodrošina mijiedarbību ar blueprintiem, etc

UCLASS(minimalapi) // samazina vajadzīgā ģenerētā koda daudzumu
class ASpeeleGameMode : public AGameModeBase // deklarē ASpeeleGameMode klasi, kas tiek mantota no Unreal
    //Engine bāzes klases AGameModeBase
{
    GENERATED_BODY() //ģenerē nepieciešamo kodu, lai Unreal Engine sistēma un 
        //citi dzinēja procesi varētu pareizi strādāt ar konkrēto klasi. Tas iekļauj dažādas deklarācijas un kodu, 
        //kas nepieciešams šiem procesiem, tādējādi atvieglojot darbu ar klasēm

public: // visu saturu var izmantot ārpus klases

    //===================================================
    // FUNKCIJAS
    //===================================================

    ASpeeleGameMode();// konstruktors objektu sākotnējo stāvokļu inicializācijai

    UFUNCTION(BlueprintCallable) // norāda, ka funkcija ir pieejama no 
        //Blueprint skriptiem un var tikt izsaukta no tiem
    void AddStartScreenWidgetToViewport(); // pievienot Menu logu ekrānam

    void BeginPlay() override; // funkcija tiek izsaukta kad spēle sākas, override nozīmē
    // ka tā tiek pārrakstīta un izpildās tas kas tajā ierakstīts šajā klasē

    void UpdateLightIntensity(); // funkcija maina gaismas vērtības spēlē

    void ToggleStartScreenWidget(); // ieslēgt un izslēgt menu ekrānu

    //===================================================
    // MAINĪGIE un ATRIBŪTI
    //===================================================

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets") // var rediģēt gan no C++ koda, gan 
    // paša editora, Blueprintos var tikai lasīt, piešķir kategoriju
    TSubclassOf<UStartScreenWidget> StartScreenWidgetClassBP; // Mainīgais kas saturēs atsauci
    // uz StartScreenWidget klases blueprintu

    UPROPERTY(BlueprintReadOnly, Category = "Widgets") // blueprintos var lasīt, kategorija widgets
    UStartScreenWidget* StartScreenWidgetInstance; // Mainīgais kas saturēs Menu Loga atsauci

private: // locekļi klases definīcijā būs pieejami tikai klases iekšienē un tos nevarēs izmantot no ārpuses
    FTimerHandle DayNightTimerHandle; // Mainīgais, kas glabā Taimera vērtību




};