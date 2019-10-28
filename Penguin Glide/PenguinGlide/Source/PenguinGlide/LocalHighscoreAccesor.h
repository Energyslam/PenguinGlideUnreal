/*!
 *  \brief     This header manages local text file accessory
 *  \details   This header declares a bunch of local text file related variables and methods
 *  \author    Tycho Zaal
 *  \version   1
 *  \date      14-04-2019
 *  \copyright TZ_productions
 */

#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "LocalHighscoreAccesor.generated.h"

/**
 * 
 */
UCLASS()
class PENGUINGLIDE_API ULocalHighscoreAccesor : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	/** \brief Loads data from text file
	*/
	UFUNCTION(BlueprintPure, Category = "Custom", meta = (Keywords = "LoadTxt"))
		static bool LoadTxt(FString FileNameA, TArray <FString>& SaveTextA);
	
	/** \brief Inserts data in text file
	*/
	UFUNCTION(BlueprintCallable, Category = "Custom", meta = (Keywords = "SaveTxt"))
		static bool SaveTxt(TArray <FString> SaveTextB, FString FileNameB);
};
