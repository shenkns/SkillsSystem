// Copyright shenkns Skills System Developed With Unreal Engine. All Rights Reserved 2023.

#include "Skills/Skill.h"

#include "Module/SkillsSystemModule.h"
#include "Components/SkillsComponent.h"
#include "LogSystem.h"
#include "Data/SkillTypeData.h"
#include "Net/UnrealNetwork.h"

USkill::USkill()
{
}

void USkill::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USkill, SkillType)
	
	if(const UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(GetClass()))
	{
		BPClass->GetLifetimeBlueprintReplicationList(OutLifetimeProps);
	}
}

UWorld* USkill::GetWorld() const
{
	return GetOuter() ? GetOuter()->GetWorld() : nullptr;
}

USkillsComponent* USkill::GetSkillsComponent() const
{
	return GetTypedOuter<USkillsComponent>();
}

AActor* USkill::GetOwner() const
{
	return GetSkillsComponent() ? GetSkillsComponent()->GetOwner() : nullptr;
}
