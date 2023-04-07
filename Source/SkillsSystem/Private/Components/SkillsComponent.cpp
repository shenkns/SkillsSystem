// Copyright shenkns Skills System Developed With Unreal Engine. All Rights Reserved 2023.

#include "Components/SkillsComponent.h"

#include "Module/SkillsSystemModule.h"
#include "LogSystem.h"
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

USkill* USkillsComponent::GetSkill(TSubclassOf<USkill> Class, USkillTypeData* Type) const
{
	USkill* const* Out = Skills.FindByPredicate([&](const USkill* Src)
	{
		return Src->GetType() == Type;
	});
	
	return Out ? *Out : nullptr;
}