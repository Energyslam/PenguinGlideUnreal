/*!
 *  \brief     This header controls the main gamemode of the game
 *  \details   This header declares all variables and methods
 *  \author    Tycho Zaal
 *  \version   1
 *  \date      08/04/2019
 *  \copyright TZ_productions
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PenguinGlideGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PENGUINGLIDE_API APenguinGlideGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blueprint Widget", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> playerHUDClass;
		TSubclassOf<class UUserWidget> mainMenuHUDClass;

	UPROPERTY()
		class UUserWidget* currentWidget;

	UFUNCTION(BlueprintCallable)
		void SetWidgetActive(TSubclassOf<class UUserWidget> activeWidget);
};
