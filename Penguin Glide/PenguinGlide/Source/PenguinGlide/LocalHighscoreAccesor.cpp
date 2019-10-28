#include "LocalHighscoreAccesor.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Runtime/Core/Public/Misc/Paths.h"

bool ULocalHighscoreAccesor::LoadTxt(FString FileName, TArray <FString>& TextToSave)
{
	return FFileHelper::LoadFileToStringArray(TextToSave, *(FPaths::GameDir() + FileName));
}

bool ULocalHighscoreAccesor::SaveTxt(TArray<FString> TextToSave, FString FileName)
{
	return FFileHelper::SaveStringArrayToFile(TextToSave, *(FPaths::GameDir() + FileName));
}
