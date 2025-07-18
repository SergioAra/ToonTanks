// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ToonTanksGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API AToonTanksGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AToonTanksGameMode();
	
	void ActorDied(AActor* DeadActor);

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bWonGame);

private:

	UPROPERTY()
	class ATank* Tank;
	UPROPERTY()
	class AToonTanksPlayerController* PlayerController;

	UPROPERTY(EditDefaultsOnly)
	float StartDelay;

	void HandleGameStart();

	int32 TargetTowers;
	int32 GetTargetTowerCount();

	
};
