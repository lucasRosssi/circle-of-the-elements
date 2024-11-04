// Copyright Lucas Rossi


#include "Managers/UpgradeManager.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/UpgradeAbility.h"
#include "AbilitySystem/Data/UpgradeInfo.h"
#include "AbilitySystem/GameplayEffects/UpgradeEffect.h"
#include "Aura/AuraLogChannels.h"
#include "Game/AuraSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "Utils/AuraSystemsLibrary.h"

void UUpgradeManager::GiveAcquiredUpgrades()
{
  if (AcquiredUpgrades.IsEmpty())
  {
    AcquiredUpgrades = GetSaveGame()->UpgradeManager.AcquiredUpgrades;
    EquippedUpgrades = GetSaveGame()->UpgradeManager.EquippedUpgrades;
  }
  else
  {
    UE_LOG(
      LogAura,
      Warning,
      TEXT("Upgrades are being overriden! If it isn't the intention, empty the AcquiredUpgrades in the game mode.")
    );
  }
  UAuraAbilitySystemComponent* AuraASC = GetAuraPlayerState()->GetAuraASC();
  
  for (const auto& [UpgradeTag, Level] : AcquiredUpgrades)
  {
    const FAuraUpgradeInfo& Info = GetUpgradeInfo()->FindUpgradeInfoByTag(UpgradeTag);
    GiveUpgrade(Info, AuraASC, Level);
    if (EquippedUpgrades.HasTagExact(UpgradeTag))
    {
      EquipUpgrade(UpgradeTag, true);
    }
  }
}

TMap<FGameplayTag, FAuraUpgradeInfo> UUpgradeManager::GetElementUpgrades(
  ECharacterName CharacterName,
  const FGameplayTag ElementTag
)
{
  return GetUpgradeInfo()->FindCharacterUpgradesOfElement(CharacterName, ElementTag);
}

FAuraUpgradeInfo UUpgradeManager::GetAuraUpgradeInfo(const FUpgradeInfoParams& Params)
{
  return GetUpgradeInfo()->FindUpgradeInfoWithParams(Params);
}

void UUpgradeManager::GetUpgradeFormattedTexts(
  const FAuraUpgradeInfo& AuraUpgradeInfo,
  int32 Level,
  bool bNextLevel,
  FText& UpgradeName,
  FText& UpgradeDescription,
  FText& UpgradeDetails
)
{
  UpgradeName = FText::FromString(FString::Printf(
    TEXT(
      "<Title>%s</>\n"
      "<Title18> Level</> <Level>%d/%d</>\n\n"
      ),
    *AuraUpgradeInfo.Name.ToString(),
    Level,
    AuraUpgradeInfo.MaxLevel
    ));

  UpgradeDescription = bNextLevel ? AuraUpgradeInfo.NextLevelDescription : AuraUpgradeInfo.Description;
  UAuraAbilitySystemLibrary::FormatUpgradeDescriptionAtLevel(
    AuraUpgradeInfo,
    Level,
    UpgradeDescription
    );
  
  FString CostText;
  for (const auto& Resource : AuraUpgradeInfo.Cost)
  {
    const FString Element = Resource.Key.ToString().Replace(TEXT("Resources.Essence."), TEXT(""));
    
    CostText += FString::Printf(
      TEXT("<%s>%s Essence</> - %d\n"),
      *Element,
      *Element,
      Resource.Value.AsInteger(Level + 1)
      );
  }
  const FString ResourcesText = FString::Printf(
    TEXT("\n<Info>Resources:</>\n%s"),
    *CostText
  );

  FString RequirementsString;
  for (auto Tag : AuraUpgradeInfo.Requirements)
  {
    const FAuraUpgradeInfo& Info = GetUpgradeInfo()->FindUpgradeInfoByTag(Tag);

    if (!Info.UpgradeAbility) continue;

    if (RequirementsString.IsEmpty())
    {
      RequirementsString += FString::Printf(TEXT("%s"), *Info.Name.ToString());
    }
    else
    {
      RequirementsString += FString::Printf(TEXT(", %s"), *Info.Name.ToString());

    }
  }
  const FString RequirementsText = FString::Printf(
    TEXT("\n<Info>Requirements: </>%s"),
    *RequirementsString
  );;

  if (AuraUpgradeInfo.Requirements.Num() > 0)
  {
    UpgradeDetails = FText::FromString(FString::Printf(TEXT(
        "\n%s"
        ),
      *RequirementsText
    ));
  }

  if (!IsMaxed(AuraUpgradeInfo))
  {
    UpgradeDetails = FText::FromString(UpgradeDetails.ToString() + FString::Printf(TEXT(
        "\n%s"
        ),
      *ResourcesText
    ));
  }
}

FString UUpgradeManager::GetUpgradeDescription(const FAuraUpgradeInfo& AuraUpgradeInfo, int32 Level)
{
  if (!AuraUpgradeInfo.IsValid()) return FString();

  const bool bNextLevel = Level > 0 && !IsMaxed(AuraUpgradeInfo) && HasResourcesToUnlock(AuraUpgradeInfo.UpgradeTag);

  FText Name;
  FText Description;
  FText Details;
  GetUpgradeFormattedTexts(AuraUpgradeInfo, Level, bNextLevel, Name, Description, Details);

  return Name.ToString() + Description.ToString() + Details.ToString();
}

bool UUpgradeManager::HasResourcesToUnlock(const FGameplayTag& UpgradeTag)
{
  if (!GetAuraPlayerState()) return false;

  const FAuraUpgradeInfo& Info = GetUpgradeInfo()->FindUpgradeInfoByTag(UpgradeTag);
  const int32 Level = AuraPlayerState->GetAuraASC()->GetAbilityLevelFromTag(Info.UpgradeAbilityTag);
  
  TMap<FGameplayTag, int32> CostMap;
  for (const auto& [Resource, ScalableCost] : Info.Cost)
  {
    CostMap.Add(Resource, ScalableCost.AsInteger(Level + 1));
  }

  return AuraPlayerState->CanAffordResourceCost(CostMap);
}

bool UUpgradeManager::HasRequiredUpgrades(const FGameplayTag& UpgradeTag)
{
  if (!GetAuraPlayerState()) return false;

  const FAuraUpgradeInfo& Info = GetUpgradeInfo()->FindUpgradeInfoByTag(UpgradeTag);

  if (Info.Requirements.IsEmpty()) return true;
  
  TArray<FGameplayTag> RequiredArray;
  Info.Requirements.GetGameplayTagArray(RequiredArray);

  for (const auto& Tag : RequiredArray)
  {
    if (AcquiredUpgrades.Contains(Tag)) return true;
  }

  return false;
}

bool UUpgradeManager::IsMaxed(const FGameplayTag& UpgradeTag)
{
  if (!IsAcquired(UpgradeTag)) return false;
  
  const FAuraUpgradeInfo& Info = GetUpgradeInfo()->FindUpgradeInfoByTag(UpgradeTag);

  return AcquiredUpgrades[UpgradeTag] >= Info.MaxLevel;
}

bool UUpgradeManager::IsMaxed(const FAuraUpgradeInfo& AuraUpgradeInfo)
{
  if (!IsAcquired(AuraUpgradeInfo.UpgradeTag)) return false;

  return AcquiredUpgrades[AuraUpgradeInfo.UpgradeTag] >= AuraUpgradeInfo.MaxLevel;
}

bool UUpgradeManager::IsAcquired(const FGameplayTag& UpgradeTag)
{
  return AcquiredUpgrades.Contains(UpgradeTag);
}

bool UUpgradeManager::IsEquipped(const FGameplayTag& UpgradeTag)
{
  return EquippedUpgrades.HasTagExact(UpgradeTag);
}

int32 UUpgradeManager::GetUpgradeLevel(const FGameplayTag& UpgradeTag)
{
  const int32* Level = AcquiredUpgrades.Find(UpgradeTag);

  return Level ? *Level : 0;
}

void UUpgradeManager::UnlockUpgrade(const FGameplayTag& UpgradeTag, int32 Level)
{
  const FAuraUpgradeInfo& Info = GetUpgradeInfo()->FindUpgradeInfoByTag(UpgradeTag);
  UAuraAbilitySystemComponent* AuraASC = GetAuraPlayerState()->GetAuraASC();

  if (IsAcquired(UpgradeTag))
  {
    if (AcquiredUpgrades[UpgradeTag] >= Info.MaxLevel) return;
    
    AcquiredUpgrades[UpgradeTag] += 1;
    
    if (Info.UpgradeAbility && Info.UpgradeAbilityTag.IsValid())
    {
      if (FGameplayAbilitySpec* AbilitySpec = AuraASC->GetSpecFromAbilityTag(Info.UpgradeAbilityTag))
      {
        if (AbilitySpec->Level >= Info.MaxLevel) return;
        
        AbilitySpec->Level = AcquiredUpgrades[UpgradeTag];
        AuraASC->MarkAbilitySpecDirty(*AbilitySpec);
      }
    }

    if (IsEquipped(UpgradeTag) && Info.UpgradeEffect)
    {
      const FGameplayTagContainer EffectTagContainer = FGameplayTagContainer({ Info.UpgradeTag });
      const FGameplayEffectQuery EffectQuery = FGameplayEffectQuery::MakeQuery_MatchAllOwningTags(EffectTagContainer);
      AuraASC->SetActiveGameplayEffectLevelUsingQuery(EffectQuery, AcquiredUpgrades[UpgradeTag]);
    }
    
    Level = AcquiredUpgrades[UpgradeTag];
  }
  else
  {
    GiveUpgrade(Info, AuraASC, Level);
    AcquiredUpgrades.Add(UpgradeTag, Level);
  }

  for (const auto& CostTuple : Info.Cost)
  {
    AuraPlayerState->AddPlayerResource(CostTuple.Key, -CostTuple.Value.AsInteger(Level));
  }

  GetSaveGame()->UpgradeManager.AcquiredUpgrades = AcquiredUpgrades;
  OnUpgradeUnlockDelegate.Broadcast(Info, Level);
}

void UUpgradeManager::EquipUpgrade(const FGameplayTag& UpgradeTag, bool bSkipMutuallyExclusiveCheck)
{
  const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
  const FAuraUpgradeInfo& Info = GetUpgradeInfo()->FindUpgradeInfoByTag(UpgradeTag);
  UAuraAbilitySystemComponent* AuraASC = GetAuraPlayerState()->GetAuraASC();

  if (!IsAcquired(UpgradeTag)) return;

  EquippedUpgrades.AddTag(UpgradeTag);

  if (Info.UpgradeAbility)
  {
    if (FGameplayAbilitySpec* UpgradeAbilitySpec = AuraASC->GetSpecFromAbilityTag(Info.UpgradeAbilityTag))
    {
      AuraASC->SetAbilityStatusFromSpec(*UpgradeAbilitySpec, AuraTags.Abilities_Status_Equipped);
      AuraASC->MarkAbilitySpecDirty(*UpgradeAbilitySpec);
      
      AuraASC->TryActivateAbilityByClass(Info.UpgradeAbility);
    }
  }
  
  const int32* Level = AcquiredUpgrades.Find(UpgradeTag);
  
  if (Info.UpgradeEffect)
  {
    const FGameplayEffectContextHandle ContextHandle = AuraASC->MakeEffectContext();
    const FGameplayEffectSpecHandle SpecHandle = AuraASC->MakeOutgoingSpec(
      Info.UpgradeEffect,
      Level ? *Level : 1, 
    ContextHandle
    );
    AuraASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
  }
  
  if (bSkipMutuallyExclusiveCheck) return; // The game is loading or we're testing
  
  for (const auto MutuallyExclusiveContainer : GetUpgradeInfo()->MutuallyExclusiveUpgrades)
  {
    if (MutuallyExclusiveContainer.HasTagExact(UpgradeTag))
    {
      for (const FGameplayTag& ContainerTag : MutuallyExclusiveContainer)
      {
        if (ContainerTag.MatchesTagExact(UpgradeTag)) continue;

        UnequipUpgrade(ContainerTag);
      }

      break;
    }
  }

  OnUpgradeUnlockDelegate.Broadcast(Info, Level ? *Level : 1);
  
  if (GetSaveGame())
  {
    SaveGame->UpgradeManager.EquippedUpgrades = EquippedUpgrades;
  }

  /* This is not working for now, ability's actor info is invalid for some reason */
  // if (Info.TargetAbilityTag.IsValid())
  // {
  //   if (const FGameplayAbilitySpec* AbilitySpec = AuraASC->GetSpecFromAbilityTag(Info.TargetAbilityTag))
  //   {
  //     IAbilityInterface::Execute_ApplyUpgrade(AbilitySpec->Ability, UpgradeTag);
  //   }
  // }
}

void UUpgradeManager::UnequipUpgrade(const FGameplayTag& UpgradeTag)
{
  const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
  const FAuraUpgradeInfo& Info = GetUpgradeInfo()->FindUpgradeInfoByTag(UpgradeTag);
  UAuraAbilitySystemComponent* AuraASC = GetAuraPlayerState()->GetAuraASC();

  EquippedUpgrades.RemoveTag(UpgradeTag);

  if (Info.UpgradeAbility)
  {
    if (FGameplayAbilitySpec* AbilitySpec = AuraASC->GetSpecFromAbilityTag(Info.UpgradeAbilityTag))
    {
      AuraASC->SetAbilityStatusFromSpec(*AbilitySpec, AuraTags.Abilities_Status_Unlocked);

      AuraASC->CancelAbility(AbilitySpec->Ability);
    }
  }
  
  if (Info.UpgradeEffect)
  {
    const FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAllOwningTags(
      FGameplayTagContainer({ UpgradeTag })
      );
    AuraASC->RemoveActiveEffects(Query);
  }

  if (GetSaveGame())
  {
    SaveGame->UpgradeManager.EquippedUpgrades = EquippedUpgrades;
  }
  
  /* This is not working for now, ability's actor info is invalid for some reason */
  // if (Info.TargetAbilityTag.IsValid())
  // {
  //   if (const FGameplayAbilitySpec* AbilitySpec = AuraASC->GetSpecFromAbilityTag(Info.TargetAbilityTag))
  //   {
  //     IAbilityInterface::Execute_RemoveUpgrade(AbilitySpec->Ability, UpgradeTag);
  //   }
  // }

}

AAuraPlayerState* UUpgradeManager::GetAuraPlayerState()
{
  if (!AuraPlayerState.IsValid())
  {
    AuraPlayerState = Cast<AAuraPlayerState>(UGameplayStatics::GetPlayerState(this, 0));
  }

  return AuraPlayerState.Get();
}

void UUpgradeManager::GiveUpgrade(
  const FAuraUpgradeInfo& AuraUpgradeInfo,
  UAuraAbilitySystemComponent* AuraASC,
  int32 Level
  )
{
  if (AuraUpgradeInfo.UpgradeAbility)
  {
    FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AuraUpgradeInfo.UpgradeAbility, Level);
    AuraASC->UnlockAbility(AbilitySpec);
  }

  /* Upgrade effects are now applied only when equipping */
  // if (AuraUpgradeInfo.UpgradeEffect)
  // {
  //   const FGameplayEffectContextHandle ContextHandle = AuraASC->MakeEffectContext();
  //   AuraASC->ApplyGameplayEffectToSelf(AuraUpgradeInfo.UpgradeEffect.GetDefaultObject(), Level, ContextHandle);
  // }
}

UUpgradeInfo* UUpgradeManager::GetUpgradeInfo()
{
  if (!UpgradeInfo)
  {
    UpgradeInfo = UAuraSystemsLibrary::GetUpgradeInfo(this);
  }

  return UpgradeInfo;
}
