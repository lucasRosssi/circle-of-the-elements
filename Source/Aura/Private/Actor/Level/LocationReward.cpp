// Copyright Lucas Rossi


#include "Actor/Level/LocationReward.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Aura/AuraLogChannels.h"
#include "Components/InteractComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Level/RewardsInfo.h"
#include "Managers/RewardManager.h"
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

void ALocationReward::Interact_Implementation(const AController* Controller)
{
  const FRewardInfo& RewardInfo = UAuraSystemsLibrary::GetRewardsInfo(this)
    ->GetRewardInfo(RewardTag);

  const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
  if (RewardTag.MatchesTag(GameplayTags.Resources_Essence))
  {
    GetAuraPlayerState()->AddPlayerResource(RewardTag, RewardInfo.Amount);
  }

  const URewardManager* RewardManager = UAuraSystemsLibrary::GetRewardManager(this);
  if (RewardManager)
  {
    RewardManager->OnRewardTakenDelegate.Broadcast();
  }

  Destroy();
}

UInteractComponent* ALocationReward::GetInteractComponent_Implementation() const
{
  return InteractComponent;
}

FGameplayEventData ALocationReward::GetAbilityEventData_Implementation() const
{
  FGameplayEventData Data;
  const FGameplayTag& ElementTag = GetAbilityElement();
  Data.EventTag = ElementTag.IsValid() ? ElementTag : RewardTag;

  return Data;
}

void ALocationReward::BeginPlay()
{
  Super::BeginPlay();

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

  if (const FGameplayTag* ElementTag = AuraTags.EssenceToAbility.Find(RewardTag))
  {
    return *ElementTag;
  }

  return FGameplayTag();
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

AAuraPlayerState* ALocationReward::GetAuraPlayerState()
{
  if (AuraPlayerState == nullptr)
  {
    AuraPlayerState = Cast<AAuraPlayerState>(UGameplayStatics::GetPlayerState(
      this,
      0)
      );
  }

  return AuraPlayerState.Get();
}
