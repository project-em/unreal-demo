// Fill out your copyright notice in the Description page of Project Settings.

#include "PennApps2016F.h"
#include "AlexaSocket.h"


void UAlexaSocket::CreateSocket() {
	FSocket* Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);
}



