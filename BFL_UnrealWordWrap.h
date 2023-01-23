/*
 * UnrealWordWrap is Unreal Engine Word Wrap written in C++ for Blueprints and is Free Unlicensed Open Source.
 * Written by Jeffrey Scott Flesher
 * https://github.com/Light-Wizzard/UnrealWordWrap
*/
#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HAL/PlatformFileManager.h"
#include <Misc/Paths.h>
#include <Misc/FileHelper.h>
#include "BFL_UnrealWordWrap.generated.h"
// ****************************************************************************
/// <summary>
/// Unreal Engine Word Wrap API
/// </summary>
UCLASS()
class UNREALWORDWRAP_API UBFL_UnrealWordWrap : public UBlueprintFunctionLibrary
{
		GENERATED_BODY()
	public:
		UFUNCTION(BlueprintPure, Category = "Custom", meta = (Keywords = "LoadTxt"))
			static bool LoadTxtFile(FString inputFileName, FString &textOut);

		UFUNCTION(BlueprintCallable, Category = "Custom", meta = (Keywords = "SaveTxt"))
			static bool SaveTxtFile(FString outputFileName, FString saveText);

		UFUNCTION(BlueprintCallable, Category = "Custom", meta = (Keywords = "IsFile"))
			static bool IsFile(FString fileName, FString &absolutePath);

		UFUNCTION(BlueprintPure, Category = "Custom", meta = (Keywords = "WrapTxt"))
			static bool WrapTxt(FString inputText, int32 wrapAt, FString &textOut);
};
// ***************************** End of File **********************************