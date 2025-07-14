// Copyright Lucas Rossi


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/AuraLogChannels.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Power);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Dodge);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalRate);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ChaosResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AirResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(WaterResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(EarthResistance);

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(
			UAuraAttributeSet,
			Power,
			Source,
			false
		);
		DEFINE_ATTRIBUTE_CAPTUREDEF(
			UAuraAttributeSet,
			Armor,
			Target,
			false
		);
		DEFINE_ATTRIBUTE_CAPTUREDEF(
			UAuraAttributeSet,
			Dodge,
			Target,
			false
		);
		DEFINE_ATTRIBUTE_CAPTUREDEF(
			UAuraAttributeSet,
			CriticalRate,
			Source,
			false
		);
		DEFINE_ATTRIBUTE_CAPTUREDEF(
			UAuraAttributeSet,
			CriticalDamage,
			Source,
			false
		);
		DEFINE_ATTRIBUTE_CAPTUREDEF(
			UAuraAttributeSet,
			AirResistance,
			Target,
			false
		);
		DEFINE_ATTRIBUTE_CAPTUREDEF(
			UAuraAttributeSet,
			ChaosResistance,
			Target,
			false
		);
		DEFINE_ATTRIBUTE_CAPTUREDEF(
			UAuraAttributeSet,
			FireResistance,
			Target,
			false
		);
		DEFINE_ATTRIBUTE_CAPTUREDEF(
			UAuraAttributeSet,
			WaterResistance,
			Target,
			false
		);
		DEFINE_ATTRIBUTE_CAPTUREDEF(
			UAuraAttributeSet,
			LightningResistance,
			Target,
			false
		);
		DEFINE_ATTRIBUTE_CAPTUREDEF(
			UAuraAttributeSet,
			EarthResistance,
			Target,
			false
		);

		const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
		TagsToCaptureDefs.Add(Tags.Attributes_Special_Power, PowerDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor, ArmorDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Dodge, DodgeDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalRate, CriticalRateDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalDamage, CriticalDamageDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Air, AirResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Chaos, ChaosResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, FireResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Water, WaterResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning, LightningResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Earth, EarthResistanceDef);
		
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().PowerDef);
	RelevantAttributesToCapture.Add(DamageStatics().DodgeDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalRateDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().AirResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ChaosResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().WaterResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().EarthResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput
	) const
{
	const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
	// const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	
	// const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;

	// Get Damage Set by Caller Magnitude
	float Damage = 0.f;

	int32 Invulnerable  = TargetASC->GetTagCount(Tags.StatusEffects_Invulnerable);
	if (Invulnerable > 0)
	{
		const FGameplayModifierEvaluatedData EvaluatedData(
		UAuraAttributeSet::GetIncomingDamageAttribute(),
		EGameplayModOp::Additive,
		Damage
	);
		OutExecutionOutput.AddOutputModifier(EvaluatedData);
		return;
	}
	
	for (const auto& Pair : Tags.DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;

	  if (!AuraDamageStatics().TagsToCaptureDefs.Contains(ResistanceTag))
	  {
	    UE_LOG(
	      LogAura,
	      Warning,
	      TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage"),
        *ResistanceTag.ToString()
        );
      
	    continue;
	  }
		
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = AuraDamageStatics()
			.TagsToCaptureDefs[ResistanceTag];

		float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag, false);
		
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
			CaptureDef,
			EvaluationParams,
			Resistance
		);
		Resistance = FMath::Min(Resistance, 1.f);

		DamageTypeValue *= 1.f - Resistance;

		// if (UAuraAbilitySystemLibrary::IsRadialDamage(EffectContextHandle))
		// {
		// 	FRadialProps RadialProps = UAuraAbilitySystemLibrary::GetRadialProps(EffectContextHandle);
		// 	DamageTypeValue = UAuraAbilitySystemLibrary::GetRadialDamageWithFalloff(
		// 		 TargetAvatar,
		// 		 DamageTypeValue,
		// 		 DamageTypeValue / 4,
		// 		 RadialProps.Origin,
		// 		 RadialProps.InnerRadius,
		// 		 RadialProps.OuterRadius,
		// 		 1.f
		// 		 );
		// }
		
		Damage += DamageTypeValue;
	}

	// Capture Dodge on Target and determine if there was a successful Parry
	// If Parry, negate damage

	float TargetDodge = 0.f;
	
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().DodgeDef,
		EvaluationParams,
		TargetDodge
	);
	TargetDodge = FMath::Max<float>(TargetDodge, 0.f);
	const bool bParried = FMath::FRandRange(0.f, 1.f) <= TargetDodge;

	UAuraAbilitySystemLibrary::SetIsParried(EffectContextHandle, bParried);

	if (bParried)
	{
		Damage = 0.f;
	}
	else
	{
		float SourcePower = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
			DamageStatics().PowerDef,
			EvaluationParams,
			SourcePower
		);

		Damage *= SourcePower;
		
		float SourceCriticalRate = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
			DamageStatics().CriticalRateDef,
			EvaluationParams,
			SourceCriticalRate
		);
		SourceCriticalRate = FMath::Max<float>(SourceCriticalRate, 0.f);
		const bool bCriticalHit = FMath::FRandRange(0.f, 1.f) <= SourceCriticalRate;

		UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);

		if (bCriticalHit)
		{
			float SourceCriticalDamage = 0.f;
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
				DamageStatics().CriticalDamageDef,
				EvaluationParams,
				SourceCriticalDamage
			);
			SourceCriticalDamage = FMath::Max<float>(SourceCriticalDamage, 1.5f);

			Damage *= SourceCriticalDamage;
		}
		
		
		float TargetArmor = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
			DamageStatics().ArmorDef,
			EvaluationParams,
			TargetArmor
		);

		Damage *= 1.f - TargetArmor;
	}

	const FGameplayModifierEvaluatedData EvaluatedData(
		UAuraAttributeSet::GetIncomingDamageAttribute(),
		EGameplayModOp::Additive,
		Damage
	);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
