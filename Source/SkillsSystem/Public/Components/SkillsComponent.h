// Copyright shenkns Skills System Developed With Unreal Engine. All Rights Reserved 2023.

#pragma once

#include "Components/ActorComponent.h"

#include "Skills/Skill.h"

#include "SkillsComponent.generated.h"

class USkillTypeData;

UCLASS(ClassGroup=(Skills), meta = (BlueprintSpawnableComponent))
class SKILLSSYSTEM_API USkillsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	USkillsComponent();

public:

	UPROPERTY(BlueprintAssignable, Category = "Skills")
	FSkillEvent OnSkillAdded;
	
	UPROPERTY(BlueprintAssignable, Category = "Skills")
	FSkillEvent OnSkillRemoved;
	
	UPROPERTY(BlueprintAssignable, Category = "Skills")
	FSkillEvent OnSkillUsed;
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Skills")
	TArray<USkillTypeData*> StartSkills;

	UPROPERTY(Replicated, VisibleInstanceOnly, Category = "Skills")
	TArray<USkill*> Skills;

public:

	virtual void BeginPlay() override;

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Skills")
	void AddSkill(USkillTypeData* SkillData);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Skills")
	void RemoveSkill(USkillTypeData* SkillData);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Skills")
	void UseSkill(USkillTypeData* SkillData);

	// Getters
	UFUNCTION(BlueprintPure, Category = "Skills", meta = (CompactNodeTitle = "Skills"))
	TArray<USkill*> GetSkills() const { return Skills; }
	
	UFUNCTION(BlueprintPure, Category = "Skills", meta = (CompactNodeTitle = "Skill", DeterminesOutputType = "Class"))
	USkill* GetSkill(TSubclassOf<USkill> Class, USkillTypeData* Type) const;

	template<typename T>
	T* GetSkill(USkillTypeData* Type) const;

private:
	
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastSkillAdded(USkill* AddedSkill);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastSkillRemoved(USkill* RemovedSkill);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastSkillUsed(USkill* UsedSkill);
};

template <typename T>
T* USkillsComponent::GetSkill(USkillTypeData* Type) const
{
	USkill* const* Out = Skills.FindByPredicate([&](const USkill* Src)
	{
		return Src->GetType() == Type;
	});

	return Out ? static_cast<T*>(*Out) : nullptr;
}
