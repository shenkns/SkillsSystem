// Copyright shenkns Skills System Developed With Unreal Engine. All Rights Reserved 2023.

#pragma once

#include "UObject/Object.h"

#include "Skill.generated.h"

class USkillsComponent;
class USkillTypeData;
class USkill;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSkillEvent, USkill*, Skill);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSkillChargeEvent, USkill*, Skill, int, NewEnergy, int, EnergyDelta);

UCLASS(BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class SKILLSSYSTEM_API USkill : public UObject
{
	GENERATED_BODY()

public:

	USkill();

public:

	UPROPERTY(BlueprintAssignable, Category = "Skill")
	FSkillEvent OnSkillUsed;

	UPROPERTY(BlueprintAssignable, Category = "Skill")
	FSkillChargeEvent OnSkillCharged;

protected:

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Skill", meta = (ExposeOnSpawn))
	USkillTypeData* SkillType;

	UPROPERTY(ReplicatedUsing = "OnRepCurrentEnergy", VisibleInstanceOnly, Category = "Skill")
	int CurrentEnergy;

	UPROPERTY(Replicated, VisibleInstanceOnly, Category = "Skill")
	bool bCooldownActive;

private:

	int OldEnergy;

	UPROPERTY()
	FTimerHandle CooldownTimer;

public:

	virtual bool IsSupportedForNetworking() const override { return true; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual UWorld* GetWorld() const override;

	void InitSkill();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Skill")
	void TryUseSkill();

	// Getters
	UFUNCTION(BlueprintPure, Category = "Skill")
	USkillTypeData* GetType() const { return SkillType; }

	UFUNCTION(BlueprintPure, Category = "Skill")
	USkillsComponent* GetSkillsComponent() const;

	UFUNCTION(BlueprintPure, Category = "Skill")
	AActor* GetOwner() const;

	UFUNCTION(BlueprintPure, Category = "Skill")
	float GetRemainingCooldown() const;

	UFUNCTION(BlueprintPure, Category = "Skill")
	int GetEnergy() const { return CurrentEnergy; }

protected:

	UFUNCTION(BlueprintNativeEvent, Category = "Skill")
	void BindSkillCharge();

	UFUNCTION(BlueprintNativeEvent, Category = "Skill")
	bool UseSkill();

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void ChargeSkill(int ChargeEnergy);

private:

	UFUNCTION()
	void OnRepCurrentEnergy();

	UFUNCTION()
	void OnCooldownFinished();
	
	void StartCooldown();
};
