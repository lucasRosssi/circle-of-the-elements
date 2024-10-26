// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AuraSystemComponent.h"
#include "UIManager.generated.h"

class AAuraHUD;
struct FGameplayTag;
class UAuraUserWidget;
class APlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewMenuOpened, UAuraUserWidget*, Widget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOpenSkillSelectionMenu, const FGameplayTag&, ElementTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOpenCharacterMenu, int32, ActiveTabIndex);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AURA_API UUIManager : public UAuraSystemComponent
{
	GENERATED_BODY()

public:	
	void SetPlayerController(APlayerController* InPlayerController);
	void SetOverlayWidget(UAuraUserWidget* InWidget);
	void SetAuraHUD(AAuraHUD* InHUD);
	UFUNCTION(BlueprintPure)
	UAuraUserWidget* GetOverlayWidget() const { return OverlayWidget.Get(); }
	UFUNCTION(BlueprintPure)
	AAuraHUD* GetAuraHUD() const { return AuraHUD.Get(); }

	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType="WidgetClass"))
	UAuraUserWidget* BuildWidget(TSubclassOf<UAuraUserWidget> WidgetClass);
	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType="Widget"))
	UAuraUserWidget* OpenWidget(UAuraUserWidget* Widget);
	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType="WidgetClass"))
	UAuraUserWidget* BuildAndOpenWidget(TSubclassOf<UAuraUserWidget> WidgetClass);

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOpenSkillSelectionMenu OpenSkillSelectionMenu;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOpenCharacterMenu OpenCharacterMenu;

	UPROPERTY(BlueprintAssignable)
	FOnNewMenuOpened OnNewMenuOpenedDelegate;
protected:
	

private:
	TWeakObjectPtr<APlayerController> PlayerController;
	TWeakObjectPtr<UAuraUserWidget> OverlayWidget;
	TWeakObjectPtr<AAuraHUD> AuraHUD;
};
