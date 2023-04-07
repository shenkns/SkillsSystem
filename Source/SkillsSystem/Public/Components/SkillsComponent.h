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
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Skills")
	TArray<USkillTypeData*> SkillsTypes;

	UPROPERTY(Replicated)
	TArray<USkill*> Skills;

public:

	virtual void BeginPlay() override;

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Getters
	UFUNCTION(BlueprintPure, Category = "Effects", meta = (CompactNodeTitle = "Effect", DeterminesOutputType = "Class"))
	USkill* GetSkill(TSubclassOf<USkill> Class, USkillTypeData* Type) const;

	template<typename T>
	T* GetSkill(USkillTypeData* Type) const;
};

template <typename T>
T* USkillsComponent::GetSkill(USkillTypeData* Type) const
{
	const USkill* Out = Skills.FindByPredicate([&](const USkill* Src)
	{
		return Src->GetType() == Type;
	});

	return Out ? static_cast<T*>(*Out) : nullptr;
}
