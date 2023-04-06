#pragma once

UENUM(BlueprintType)
enum class ETurretState :uint8
{
	ETS_NonCombat UMETA(DisplayName = "NonCombat"),
	ETS_Searching UMETA(DisplayNmae = "Searching"),
	ETS_InCombat UMETA(DisplayName = "InCombat"),
	ETS_Destroyed UMETA(DisplayName = "Destroyed"),

	ETS_MAX UMETA(DisplayName = "DefaultMAX")
};