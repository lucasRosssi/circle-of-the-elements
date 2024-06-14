// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputState.h"
#include "Engine/DataAsset.h"
#include "AuraInputConfig.generated.h"

USTRUCT(BlueprintType)
struct FInputIcons
{
	GENERATED_BODY()

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		meta=( 
			DisplayThumbnail="true",
			AllowedClasses="/Script/Engine.Texture,/Script/Engine.MaterialInterface,/Script/Engine.SlateTextureAtlasInterface",
			DisallowedClasses = "/Script/MediaAssets.MediaTexture"
		)
	)
	TObjectPtr<UObject> KeyboardIcon;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		meta=( 
			DisplayThumbnail="true",
			AllowedClasses="/Script/Engine.Texture,/Script/Engine.MaterialInterface,/Script/Engine.SlateTextureAtlasInterface",
			DisallowedClasses = "/Script/MediaAssets.MediaTexture"
		)
	)
	TObjectPtr<UObject> PlaystationIcon;
};

USTRUCT(BlueprintType)
struct FAuraInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FInputIcons InputIcons = FInputIcons();
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	const UInputAction* FindAbilityInputActionForTag(
		const FGameplayTag& InputTag,
		bool bLogNotFound = false) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (HidePin = "Target", DefaultToSelf = "Target"))
	FInputIcons FindInputIconsByTag(
		const FGameplayTag& InputTag,
		bool bLogNotFound = false
	) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(TitleProperty="InputTag"))
	TArray<FAuraInputAction> AbilityInputActions;
};
