// Copyright Lucas Rossi


#include "Managers/UpgradeManager.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/UpgradeAbility.h"
#include "AbilitySystem/Data/UpgradeInfo.h"
#include "AbilitySystem/GameplayEffects/UpgradeEffect.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "Utils/AuraSystemsLibrary.h"

void UUpgradeManager::GiveAcquiredUpgrades()
{
  if (AcquiredUpgrades.Num() > 0)
  {
    for (auto UpgradeTagIterator = AcquiredUpgrades.CreateConstIterator(); UpgradeTagIterator; ++UpgradeTagIterator)
    {
      UnlockUpgrade(*UpgradeTagIterator);
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

    if (!Info.Ability) continue;

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

FString UUpgradeManager::GetUpgradeDescription(
  const FAuraUpgradeInfo& AuraUpgradeInfo,
  int32 Level,
  bool bNextLevel
  )
{
  if (!AuraUpgradeInfo.IsValid()) return FString();

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
  const int32 Level = AuraPlayerState->GetAuraASC()->GetAbilityLevelFromTag(Info.AbilityTag);
  
  TMap<FGameplayTag, int32> CostMap;
  for (const auto& [Resource, ScalableCost] : Info.Cost)
  {
    CostMap.Add(Resource, ScalableCost.AsInteger(Level));
  }

  return AuraPlayerState->CanAffordResourceCost(CostMap);
}

bool UUpgradeManager::HasRequiredUpgrades(const FGameplayTag& UpgradeTag)
{
  if (!GetAuraPlayerState()) return false;

  const FAuraUpgradeInfo& Info = GetUpgradeInfo()->FindUpgradeInfoByTag(UpgradeTag);
  
  return AcquiredUpgrades.HasAllExact(Info.Requirements);
}

bool UUpgradeManager::IsMaxed(const FGameplayTag& UpgradeTag)
{
  const FAuraUpgradeInfo& Info = GetUpgradeInfo()->FindUpgradeInfoByTag(UpgradeTag);
  UAuraAbilitySystemComponent* AuraASC = GetAuraPlayerState()->GetAuraASC();
  
  if (const FGameplayAbilitySpec* AbilitySpec = AuraASC->GetSpecFromAbilityTag(Info.AbilityTag))
  {
    return AbilitySpec->Level >= Info.MaxLevel;
  }

  return false;
}

bool UUpgradeManager::IsMaxed(const FAuraUpgradeInfo& AuraUpgradeInfo)
{
  UAuraAbilitySystemComponent* AuraASC = GetAuraPlayerState()->GetAuraASC();

  if (const FGameplayAbilitySpec* AbilitySpec = AuraASC->GetSpecFromAbilityTag(AuraUpgradeInfo.AbilityTag))
  {
    return AbilitySpec->Level >= AuraUpgradeInfo.MaxLevel;
  }

  return false;
}

void UUpgradeManager::UnlockUpgrade(const FGameplayTag& UpgradeTag)
{
  const FAuraUpgradeInfo& Info = GetUpgradeInfo()->FindUpgradeInfoByTag(UpgradeTag);
  UAuraAbilitySystemComponent* AuraASC = GetAuraPlayerState()->GetAuraASC();

  int32 Level;
  if (FGameplayAbilitySpec* AbilitySpec = AuraASC->GetSpecFromAbilityTag(Info.AbilityTag))
  {
    if (AbilitySpec->Level >= Info.MaxLevel) return;
    
    AbilitySpec->Level += 1;
    AuraASC->MarkAbilitySpecDirty(*AbilitySpec);

    Level = AbilitySpec->Level;

    const FGameplayTagContainer EffectTagContainer = FGameplayTagContainer({ Info.UpgradeTag });
    const FGameplayEffectQuery EffectQuery = FGameplayEffectQuery::MakeQuery_MatchAllOwningTags(EffectTagContainer);
    AuraASC->SetActiveGameplayEffectLevelUsingQuery(EffectQuery, Level);
  }
  else
  {
    GiveUpgrade(Info, AuraASC);
    AcquiredUpgrades.AddTag(UpgradeTag);

    Level = 1;
  }

  for (const auto& CostTuple : Info.Cost)
  {
    AuraPlayerState->AddPlayerResource(CostTuple.Key, -CostTuple.Value.AsInteger(Level));
  }
  
  OnUpgradeUnlockDelegate.Broadcast(Info, Level);
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
  UAuraAbilitySystemComponent* AuraASC
  )
{
  if (AuraUpgradeInfo.Ability)
  {
    FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AuraUpgradeInfo.Ability, 1);
    AuraASC->UnlockAbility(AbilitySpec);
  }

  if (AuraUpgradeInfo.UpgradeEffect)
  {
    const FGameplayEffectContextHandle ContextHandle = AuraASC->MakeEffectContext();
    AuraASC->ApplyGameplayEffectToSelf(AuraUpgradeInfo.UpgradeEffect.GetDefaultObject(), 1, ContextHandle);
  }
}

UUpgradeInfo* UUpgradeManager::GetUpgradeInfo()
{
  if (!UpgradeInfo)
  {
    UpgradeInfo = UAuraSystemsLibrary::GetUpgradeInfo(this);
  }

  return UpgradeInfo;
}
