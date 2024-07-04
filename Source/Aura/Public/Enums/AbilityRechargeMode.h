#pragma once

UENUM(BlueprintType)
enum class EAbilityRechargeMode : uint8
{
	// One charge at a time, cooldown effect needs to be stackable and not refreshing duration
	OneCharge UMETA(DisplayName = "One Charge"),
	// All charges at once, cooldown effect should not stack
	AllCharges UMETA(DisplayName = "All Charges"),

	MAX UMETA(Hidden)
};
