// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "AmazonRESTClient.generated.h"

/**
 * 
 */
UCLASS()
class UAmazonRESTClient : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintReadWrite, EditAnywhere)
	void searchItems(const FString *text);

private:
	static FString AMAZON_URL;
	
	
};
