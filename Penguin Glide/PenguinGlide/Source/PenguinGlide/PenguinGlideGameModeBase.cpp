/*!
 *  \brief     This class controls the main gamemode of the game
 *  \details   This class inizializes and handles all variables and methods
 *  \author    Tycho Zaal
 *  \version   1
 *  \date      08/04/2019
 *  \copyright TZ_productions
 */

#include "PenguinGlideGameModeBase.h"
#include "Blueprint/UserWidget.h"

void APenguinGlideGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

/** \brief This is called when a level's BeginPlay is being called.
*
*/
void APenguinGlideGameModeBase::SetWidgetActive(TSubclassOf<class UUserWidget> activeWidget)
{
	if (activeWidget)
	{
		currentWidget = CreateWidget<UUserWidget>(GetWorld(), activeWidget);

		if (currentWidget)
		{
			currentWidget->AddToViewport();
		}
	}
}


