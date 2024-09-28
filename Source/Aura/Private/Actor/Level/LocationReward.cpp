// Copyright Lucas Rossi


#include "Actor/Level/LocationReward.h"

#include "AuraGameplayTags.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/AuraLogChannels.h"
#include "Components/InteractComponent.h"
#include "Game/AuraGameInstance.h"
#include "Game/AuraGameModeBase.h"
#include "Level/RewardsInfo.h"
#include "Managers/RewardManager.h"
#include "Managers/UIManager.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "Utils/AuraSystemsLibrary.h"

ALocationReward::ALocationReward()
{
  RewardMesh = CreateDefaultSubobject<UStaticMeshComponent>("RewardMesh");
  SetRootComponent(RewardMesh);
  
  InteractComponent = CreateDefaultSubobject<UInteractComponent>("InteractComponent");
  InteractComponent->SetupInteractAreaAttachment(GetRootComponent());
  InteractComponent->EnableInteraction();
}

void ALocationReward::BeginPlay()
{
  Super::BeginPlay();

  InteractComponent->OnInteractedDelegate.BindUObject(this, &ALocationReward::Interact);
  InteractComponent->OnPostInteractedDelegate.BindUObject(this, &ALocationReward::OnInteracted);

  SpawnNiagaraEffects();
}

void ALocationReward::Destroyed()
{
  if (IdleNiagaraComponent)
  {
    IdleNiagaraComponent->Deactivate();
  }

  Super::Destroyed();
}

void ALocationReward::Interact(const AAuraPlayerController* InstigatorController)
{
  const FRewardInfo& RewardInfo = UAuraAbilitySystemLibrary::GetRewardsInfo(this)
    ->GetRewardInfo(RewardTag);

  const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
  if (RewardTag.MatchesTag(GameplayTags.Resources_Essence))
  {
    AAuraPlayerState* AuraPS = InstigatorController->GetPlayerState<AAuraPlayerState>();

    if (RewardTag.MatchesTagExact(GameplayTags.Resources_Essence_Soul))
    {
      AuraPS->AddAttributePoints(RewardInfo.Amount);
    }
    else
    {
      InstigatorController->GetUIManager()->OpenSkillSelectionMenu.Broadcast(GetAbilityElement());
    }

    GetAuraGameInstance()->AddPlayerResource(RewardTag, RewardInfo.Amount);
  }

  const URewardManager* RewardManager = UAuraSystemsLibrary::GetRewardManager(this);
  RewardManager->OnRewardTakenDelegate.Broadcast();
}

FGameplayTag ALocationReward::GetAbilityElement() const
{
  const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
  if (!RewardTag.MatchesTag(AuraTags.Resources_Essence))
  {
    UE_LOG(
      LogAura,
      Error,
      TEXT(
        "Reward must be an Essence to get its corresponding Ability Element."
        "RewardTag is: %s"
      ),
      *RewardTag.ToString()
    );
    return FGameplayTag();
  }

  return AuraTags.EssenceToAbility[RewardTag];
}

void ALocationReward::SpawnNiagaraEffects()
{
  if (SpawnNiagaraSystem)
  {
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(
      this,
      SpawnNiagaraSystem,
      GetActorLocation() + SpawnNiagaraOffset
    );
  }
  if (IdleNiagaraSystem)
  {
    IdleNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
      this,
      IdleNiagaraSystem,
      GetActorLocation() + IdleNiagaraOffset
    );
  }
}

UAuraGameInstance* ALocationReward::GetAuraGameInstance()
{
  if (AuraGameInstance == nullptr)
  {
    AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
  }

  return AuraGameInstance.Get();
}
