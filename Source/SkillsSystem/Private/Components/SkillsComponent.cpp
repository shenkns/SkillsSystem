// Copyright shenkns Skills System Developed With Unreal Engine. All Rights Reserved 2023.

#include "Components/SkillsComponent.h"

#include "Module/SkillsSystemModule.h"
#include "LogSystem.h"
#include "Data/SkillTypeData.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

USkillsComponent::USkillsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	SetIsReplicatedByDefault(true);
}

void USkillsComponent::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwner()->HasAuthority())
	{
		for(USkillTypeData* SkillTypeData : StartSkills)
		{
			AddSkill(SkillTypeData);
		}
	}
}

bool USkillsComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for(USkill* Skill : Skills)
	{
		if(Skill)
		{
			bWroteSomething |= Channel->ReplicateSubobject(Skill, *Bunch, *RepFlags);
		}
	}

	return bWroteSomething;
}

void USkillsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(USkillsComponent, Skills)
}

void USkillsComponent::AddSkill(USkillTypeData* SkillData)
{
	if(!SkillData || !SkillData->SkillClass) return;
	if(GetSkill<USkill>(SkillData)) return;

	USkill* Skill = NewObject<USkill>(this, SkillData->SkillClass);
	Skill->OnSkillUsed.AddUniqueDynamic(this, &USkillsComponent::SkillUsed);

	Skill->InitSkill();

	Skills.Add(Skill);

	OnSkillAdded.Broadcast(Skill);
}

void USkillsComponent::RemoveSkill(USkillTypeData* SkillData)
{
	if(USkill* Skill = GetSkill<USkill>(SkillData))
	{
		Skill->OnSkillUsed.RemoveDynamic(this, &USkillsComponent::SkillUsed);
		Skills.Remove(Skill);

		OnSkillRemoved.Broadcast(Skill);

		Skill->Rename(nullptr, nullptr);
	}
}

void USkillsComponent::UseSkill(USkillTypeData* SkillData)
{
	if(USkill* Skill = GetSkill<USkill>(SkillData))
	{
		Skill->TryUseSkill();
	}
}

USkill* USkillsComponent::GetSkill(TSubclassOf<USkill> Class, USkillTypeData* Type) const
{
	USkill* const* Out = Skills.FindByPredicate([&](const USkill* Src)
	{
		return Src->GetType() == Type;
	});
	
	return Out ? *Out : nullptr;
}

void USkillsComponent::SkillUsed(USkill* Skill)
{
	OnSkillUsed.Broadcast(Skill);
}
