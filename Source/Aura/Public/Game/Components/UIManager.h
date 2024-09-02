// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UIManager.generated.h"


struct FGameplayTag;
class UAuraUserWidget;
class AAuraPlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewMenuOpened, UAuraUserWidget*, Widget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOpenSkillSelectionMenu, const FGameplayTag&, ElementTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOpenCharacterMenu, int32, ActiveTabIndex);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AURA_API UUIManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUIManager();

	void SetAuraPlayerController(AAuraPlayerController* InController) { AuraPlayerController = InController; }
	AAuraPlayerController* GetAuraPlayerController() const { return AuraPlayerController; }
	UFUNCTION(BlueprintPure)
	UAuraUserWidget* GetOverlayWidget();

	UFUNCTION(BlueprintCallable)
	UAuraUserWidget* BuildWidget(TSubclassOf<UAuraUserWidget> WidgetClass);
	UFUNCTION(BlueprintCallable)
	void OpenWidget(UAuraUserWidget* Widget);
	UFUNCTION(BlueprintCallable)
	UAuraUserWidget* BuildAndOpenWidget(TSubclassOf<UAuraUserWidget> WidgetClass);

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOpenSkillSelectionMenu OpenSkillSelectionMenu;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOpenCharacterMenu OpenCharacterMenu;

	UPROPERTY(BlueprintAssignable)
	FOnNewMenuOpened OnNewMenuOpenedDelegate;
protected:
	

private:
	UPROPERTY()
	AAuraPlayerController* AuraPlayerController = nullptr;
	UPROPERTY()
	UAuraUserWidget* OverlayWidget = nullptr;
};
