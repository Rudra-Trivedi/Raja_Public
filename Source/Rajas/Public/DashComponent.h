// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "DashComponent.generated.h"


UCLASS( ClassGroup=(Dash), meta=(BlueprintSpawnableComponent) )
class RAJAS_API UDashComponent : public UActorComponent
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this component's properties
	UDashComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	

private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Dash")
	float DashDistance = 100.0f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Dash")
	float DashTime = 1.0f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Dash")
	float MaxDashTime = 1.0f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Dash")
	float MinDashTime = 0.01f;

	UPROPERTY(EditAnywhere, meta = (AllowprivateAccess = "true"), Category = "Dash")
	UCurveFloat* FloatCurve;

	
	FTimeline LerpingTimeline;

	UPROPERTY()
	class UCameraComponent* cameraComponent;

	UPROPERTY()
	class UMovementComponent* PlayerMovementComponent;

	//float StartTime = 0.f;

	bool bLerping = false;

	FVector StartPos;

	FVector FinalPos;

	AActor* ParentActor = nullptr;

	//FTimerHandle LerpTimerHandle;

	UFUNCTION()
	void OnTimelineFloat(float value);

	UFUNCTION(Category = "Dash")
	void SetFinalPos();

	
	UFUNCTION()
	float GetTimeMultiplier();


public:

	UFUNCTION(BlueprintCallable, Category = "Dash")
	void Dash();
	
	

	FORCEINLINE bool IsLerping() const { return bLerping; }
		
};
