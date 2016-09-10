// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Networking.h"
#include "TCPSocket.generated.h"

UCLASS()
class PENNAPPS2016F_API ATCPSocket : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATCPSocket();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category="Socket")
	void CreateSocket();

	UFUNCTION(BlueprintCallable, Category = "Socket")
	void SendMessage();

	UFUNCTION(BLueprintCallable, Category = "Socket")
	void AcceptClient();
	
	UPROPERTY(BlueprintReadWrite, Category = "Socket")
	int32 port;

	UPROPERTY(BlueprintReadWrite, Category = "Socket")
	FString address;

	FSocket* Socket;
	FSocket *clientSocket;
	
	
};
