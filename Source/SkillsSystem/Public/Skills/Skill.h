// Copyright shenkns Skills System Developed With Unreal Engine. All Rights Reserved 2023.

#pragma once

#include "UObject/Object.h"

#include "Skill.generated.h"

class USkillsComponent;
class USkillTypeData;

UCLASS(BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class SKILLSSYSTEM_API USkill : public UObject
{
	GENERATED_BODY()

public:

	USkill();

protected:

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Skill", meta = (ExposeOnSpawn))
	USkillTypeData* SkillType;

public:

	virtual bool IsSupportedForNetworking() const override { return true; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual UWorld* GetWorld() const override;

	// Getters
	UFUNCTION(BlueprintPure, Category = "Skill")
	USkillTypeData* GetType() const { return SkillType; }

	UFUNCTION(BlueprintPure, Category = "Skill")
	USkillsComponent* GetSkillsComponent() const;

	UFUNCTION(BlueprintPure, Category = "Skill")
	AActor* GetOwner() const;
};
