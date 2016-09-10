// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Networking.h"
#include "AlexaSocket.generated.h"

/**
 * 
 */
UCLASS()
class PENNAPPS2016F_API UAlexaSocket : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Socket")
	void CreateSocket();
	
	
	
	
};
