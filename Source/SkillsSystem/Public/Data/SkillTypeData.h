// Copyright shenkns Skills System Developed With Unreal Engine. All Rights Reserved 2023.

#pragma once

#include "Data/Data.h"

#include "SkillTypeData.generated.h"

class USkill;

UCLASS()
class SKILLSSYSTEM_API USkillTypeData : public UData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
	TSubclassOf<USkill> SkillClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
	bool bEnableCooldown;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill", meta = (EditCondition = "bEnableCooldown", EditConditionHides))
	int Cooldown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill", meta = (EditCondition = "bEnableCooldown", EditConditionHides))
	bool bEnableStartCooldown;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
	int MaxEnergy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
	int UseEnergy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
	bool bStartCharged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill", meta = (EditCondition = "!bStartCharged", EditConditionHides))
	int StartEnergy;
};
