// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TradingCenter.generated.h"

UCLASS()
class SPEELE_API ATradingCenter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATradingCenter();

	// Add a variable to hold a reference to the trading panel widget
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	class UTradingPanel* TradingPanel;

	bool bPlayerInside;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	bool bTradingPanelVisible;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void ToggleTradingPanel();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> InteractionWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> TradingPanelClass;

private:
	UUserWidget* InteractionWidgetInstance;
	

};
