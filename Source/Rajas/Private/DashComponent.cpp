// Fill out your copyright notice in the Description page of Project Settings.


#include "DashComponent.h"
#include "Components/TimelineComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UDashComponent::UDashComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDashComponent::BeginPlay()
{
	Super::BeginPlay();

	ParentActor = GetOwner();
	if (ParentActor)
	{
		cameraComponent = ParentActor->FindComponentByClass<UCameraComponent>();
		//PlayerMovementComponent = ParentActor->FindComponentByClass<UMovementComponent>();
	}
	
	//LerpingTimeline = NewObject<UTimelineComponent>(this, FName("LerpingTimeline"));
	//LerpingTimeline = CreateDefaultSubobject<UTimelineComponent>(FName("LerpingTimeline"));


	FOnTimelineFloat TimelineCallback;
	TimelineCallback.BindUFunction(this, FName("OnTimelineFloat"));
	LerpingTimeline.AddInterpFloat(FloatCurve, TimelineCallback);

	

}



// Called every frame
void UDashComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bLerping)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("LERPING"));
		LerpingTimeline.TickTimeline(DeltaTime);
	}
	
}

void UDashComponent::Dash()
{
	

	ParentActor->DisableInput(GetWorld()->GetFirstPlayerController());

	StartPos = ParentActor->GetActorLocation();
	
	SetFinalPos();

	float DashTimeMultiplier = GetTimeMultiplier();

	LerpingTimeline.SetTimelineLength( DashTime * DashTimeMultiplier );

	LerpingTimeline.PlayFromStart();

	bLerping = true;
}

void UDashComponent::SetFinalPos()
{
	if (cameraComponent)
	{
		FinalPos = StartPos + (cameraComponent->GetForwardVector() * DashDistance);
		FinalPos.Z = FMath::Max(FinalPos.Z, 100.f);
	}

	FHitResult hitResult;

	bool bHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartPos, FinalPos, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, hitResult, true);

	if (bHit)
	{
		FinalPos = hitResult.ImpactPoint - (cameraComponent->GetForwardVector() - 50.f);
		FinalPos.Z = FMath::Max(FinalPos.Z, 100.f);

	}

	DrawDebugSphere(GetWorld(), FinalPos, 20.f, 12, FColor::Red, false, 2.0f, 0, 1.f);
}

float UDashComponent::GetTimeMultiplier()
{
	float Distance = FVector::Distance(FinalPos, StartPos);
	float Multiplier = FMath::Clamp(Distance / DashDistance, 0.0f, 1.0f);

	return FMath::Lerp(MinDashTime, MaxDashTime, Multiplier);
}

void UDashComponent::OnTimelineFloat(float value)
{
	float LerpFactor = FloatCurve->GetFloatValue(value);
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::SanitizeFloat(LerpFactor));
	//FVector NewLocation = StartPos + (ParentActor->GetActorForwardVector() * DashDistance * LerpFactor);
	FVector NewLocation = FMath::Lerp(StartPos, FinalPos, LerpFactor);
	ParentActor->SetActorLocation(NewLocation);
	if (LerpFactor >= 1)
	{
		bLerping = false;
		ParentActor->EnableInput(GetWorld()->GetFirstPlayerController());
	}
}



//void UDashComponent::HandleLerpTimerTick()
//{
//	if (!ParentActor)
//	{
//		return;
//	}
//	
//	float dt = GetWorld()->GetRealTimeSeconds() - StartTime;
//
//	float alpha = FMath::Clamp((dt / DashTime), 0.0f, 1.0f);
//
//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::SanitizeFloat(alpha));
//
//	FVector NewLocation = FMath::Lerp(StartPos, FinalPos, alpha);
//	ParentActor->SetActorLocation(NewLocation);
//
//	if (alpha >= 1.0f)
//	{
//		ParentActor->GetWorldTimerManager().ClearTimer(LerpTimerHandle);
//		bLerping = false;
//		ParentActor->EnableInput(GetWorld()->GetFirstPlayerController());
//	}
//}



