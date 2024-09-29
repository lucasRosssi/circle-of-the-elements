// Copyright Lucas Rossi


#include "Player/AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/LevelInfo.h"
#include "Character/AuraCharacterBase.h"
#include "Interfaces/PlayerInterface.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>
	("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated((true));
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	
	NetUpdateFrequency = 100.f;
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuraPlayerState, Level);
	DOREPLIFETIME(AAuraPlayerState, XP);
	DOREPLIFETIME(AAuraPlayerState, AttributePoints);
	DOREPLIFETIME(AAuraPlayerState, SkillPoints);
	DOREPLIFETIME(AAuraPlayerState, PerkPoints);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAuraAbilitySystemComponent* AAuraPlayerState::GetAuraASC()
{
	if (AuraASC == nullptr)
	{
		AuraASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent());
	}
	
	return AuraASC;
}

void AAuraPlayerState::SetActionSpeed_Implementation(float InActionSpeed)
{
	if (const auto Character = GetCharacterBase())
	{
		Character->ChangeActionSpeed(InActionSpeed);
	}
}

float AAuraPlayerState::GetPower_Implementation()
{
	return AttributeSet->GetPower();
}

void AAuraPlayerState::SetTimeDilation_Implementation(float InTimeDilation)
{
	GetPawn()->CustomTimeDilation = InTimeDilation;
}

void AAuraPlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void AAuraPlayerState::AddLevel(int32 InLevel)
{
	if (InLevel < 1) return;
	
	const int32 OldLevel = Level;
	Level += InLevel;
	IPlayerInterface::Execute_LevelUp(AbilitySystemComponent->GetAvatarActor());
	OnLevelChangedDelegate.Broadcast(Level);
	
	const int32 APReward = LevelInfo.Get()->GetLevelUpAttributePoints(OldLevel, Level);
	const int32 SPReward = LevelInfo.Get()->GetLevelUpSkillPoints(OldLevel, Level);

	AddAttributePoints(APReward);
	AddSkillPoints(SPReward);

	// if (GetAuraASC())
	// {
	// 	GetAuraASC()->UpdateAbilityStatuses(Level);
	// }
}

void AAuraPlayerState::SetXP(int32 InXP)
{
	if (XP == InXP) return;
	
	XP = InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::AddXP(int32 InXP)
{
	if (InXP < 1) return;
	
	XP += InXP;
	OnXPChangedDelegate.Broadcast(XP);
	const int32 LevelUpAmount = LevelInfo.Get()->FindLevelByXP(XP) - Level;

	if (LevelUpAmount > 0)
	{
		AddLevel(LevelUpAmount);
	}
}

void AAuraPlayerState::SetAttributePoints(int32 InAttributePoints)
{
	if (AttributePoints == InAttributePoints) return;
	
	AttributePoints = InAttributePoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::AddAttributePoints(int32 InAttributePoints)
{
	if (InAttributePoints == 0) return;
	
	AttributePoints += InAttributePoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::SetSkillPoints(int32 InSkillPoints)
{
	if (SkillPoints == InSkillPoints) return;
	
	SkillPoints = InSkillPoints;
	OnSkillPointsChangedDelegate.Broadcast(SkillPoints);
}

void AAuraPlayerState::AddSkillPoints(int32 InSkillPoints)
{
	if (InSkillPoints == 0) return;
	
	SkillPoints += InSkillPoints;
	OnSkillPointsChangedDelegate.Broadcast(SkillPoints);
}

void AAuraPlayerState::SetPerkPoints(int32 InPerkPoints)
{
	if (PerkPoints == InPerkPoints) return;
	
	PerkPoints = InPerkPoints;
	OnPerkPointsChangedDelegate.Broadcast(PerkPoints);
}

void AAuraPlayerState::AddPerkPoints(int32 InPerkPoints)
{
	if (InPerkPoints == 0) return;
	
	PerkPoints += InPerkPoints;
	OnPerkPointsChangedDelegate.Broadcast(PerkPoints);
}

AAuraCharacterBase* AAuraPlayerState::GetCharacterBase()
{
	if (CharacterBase == nullptr)
	{
		CharacterBase = Cast<AAuraCharacterBase>(GetPawn());
	}

	return CharacterBase;
}

void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level);
}

void AAuraPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::OnRep_SkillPoints(int32 OldSkillPoints)
{
	OnSkillPointsChangedDelegate.Broadcast(SkillPoints);
}

void AAuraPlayerState::OnRep_PerkPoints(int32 OldPerkPoints)
{
	OnPerkPointsChangedDelegate.Broadcast(PerkPoints);
}
