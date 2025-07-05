// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraInputConfig.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FAuraInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};

USTRUCT(BlueprintType)
struct FAuraMenuInput
{
  GENERATED_BODY()

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  UInputAction* InputAction = nullptr;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  FText Label = FText();

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Brush, meta=( DisplayThumbnail="true", 
  AllowedClasses="/Script/Engine.Texture,/Script/Engine.MaterialInterface,/Script/Engine.SlateTextureAtlasInterface", DisallowedClasses = "/Script/MediaAssets.MediaTexture"))
  TObjectPtr<UObject> GamepadImage;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Brush, meta=( DisplayThumbnail="true", 
  AllowedClasses="/Script/Engine.Texture,/Script/Engine.MaterialInterface,/Script/Engine.SlateTextureAtlasInterface", DisallowedClasses = "/Script/MediaAssets.MediaTexture"))
  TObjectPtr<UObject> KeyboardMouseImage;
};

USTRUCT(BlueprintType)
struct FMenuInputs
{
  GENERATED_BODY()

  UPROPERTY(EditDefaultsOnly, meta=(TitleProperty="Label"))
  TArray<FAuraMenuInput> Inputs;
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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(TitleProperty="InputTag"))
	TArray<FAuraInputAction> AbilityInputActions;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(
    ShowOnlyInnerProperties,
    NoResetToDefault,
    ForceInlineRow
    )
  )
  TMap<FGameplayTag, FMenuInputs> MenuInputs;
};
