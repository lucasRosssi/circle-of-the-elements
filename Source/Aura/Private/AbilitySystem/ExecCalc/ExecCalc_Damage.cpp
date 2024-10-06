// Copyright Lucas Rossi


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Power);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ParryChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalRate);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(EnergyResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(IceResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(NecroticResistance);

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
			ParryChance,
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
			PhysicalResistance,
			Target,
			false
		);
		DEFINE_ATTRIBUTE_CAPTUREDEF(
			UAuraAttributeSet,
			EnergyResistance,
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
			IceResistance,
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
			NecroticResistance,
			Target,
			false
		);

		const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
		TagsToCaptureDefs.Add(Tags.Attributes_Special_Power, PowerDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor, ArmorDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_ParryChance, ParryChanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalRate, CriticalRateDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalDamage, CriticalDamageDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, PhysicalResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Energy, EnergyResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, FireResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Ice, IceResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning, LightningResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Necrotic, NecroticResistanceDef);
		
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
	RelevantAttributesToCapture.Add(DamageStatics().ParryChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalRateDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().EnergyResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().IceResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().NecroticResistanceDef);
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
		checkf(
			AuraDamageStatics().TagsToCaptureDefs.Contains(ResistanceTag),
			TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage"),
			*ResistanceTag.ToString()
		);
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

		if (UAuraAbilitySystemLibrary::IsRadialDamage(EffectContextHandle))
		{
			FRadialProps RadialProps = UAuraAbilitySystemLibrary::GetRadialProps(EffectContextHandle);
			DamageTypeValue = UAuraAbilitySystemLibrary::GetRadialDamageWithFalloff(
				 TargetAvatar,
				 DamageTypeValue,
				 DamageTypeValue / 4,
				 RadialProps.Origin,
				 RadialProps.InnerRadius,
				 RadialProps.OuterRadius,
				 1.f
				 );
		}
		
		Damage += DamageTypeValue;
	}

	// Capture ParryChance on Target and determine if there was a successful Parry
	// If Parry, negate damage

	float TargetParryChance = 0.f;
	
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().ParryChanceDef,
		EvaluationParams,
		TargetParryChance
	);
	TargetParryChance = FMath::Max<float>(TargetParryChance, 0.f);
	const bool bParried = FMath::FRandRange(0.f, 1.f) <= TargetParryChance;

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
