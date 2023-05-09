#pragma once

UENUM(BlueprintType)
enum class ETurretBuildState :uint8
{
	ETBS_OnBuildGreen UMETA(DisplayName = "OnBuildGreen"),
	ETBS_OnBuildRed UMETA(DisplayName = "OnBuildRed"),
	ETBS_BuildCompleted UMETA(DisplayName = "BuildCompleted"),

	ETBS_MAX UMETA(DisplayName = "DefaultMAX")
};