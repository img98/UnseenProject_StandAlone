#pragma once

UENUM(BlueprintType)
enum class EBuildState :uint8
{
	EBS_OnBuildGreen UMETA(DisplayName = "OnBuildGreen"),
	EBS_OnBuildRed UMETA(DisplayName = "OnBuildRed"),
	EBS_BuildCompleted UMETA(DisplayName = "BuildCompleted"),

	EBS_MAX UMETA(DisplayName = "DefaultMAX")
};