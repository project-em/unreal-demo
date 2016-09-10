// Fill out your copyright notice in the Description page of Project Settings.

#include "PennApps2016F.h"
#include "SocketThread.h"

// Singleton
FSocketThread* FSocketThread::Runnable = NULL;
FSocketThread* FSocketThread::Singleton(FSocket* socket, FAlexaEvent* alexaEvent)
{
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new FSocketThread(socket, alexaEvent);
	}
	return Runnable;
}

FSocketThread::FSocketThread(FSocket* socket, FAlexaEvent* alexaEvent)
	: socket(socket), alexaEvent(alexaEvent), StopTaskCounter(0)
{
	Thread = FRunnableThread::Create(this, TEXT("FSocketThread"), 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
}

FSocketThread::~FSocketThread()
{
	delete Thread;
	Thread = NULL;
}

bool FSocketThread::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Socket running on %d"), socket->GetPortNo());
	clientSocket = socket->Accept(TEXT("Connected to client.:"));
	if (clientSocket)
		UE_LOG(LogTemp, Warning, TEXT("Connected to client running on %d"), clientSocket->GetPortNo());
	return clientSocket != NULL;
}

uint32 FSocketThread::Run()
{
	//Initial wait before starting
	FPlatformProcess::Sleep(0.03);
	TArray<uint8> buf;
	int32 bytesRead = 0;
	uint32 pendingDataSize = 0;
	while (StopTaskCounter.GetValue() == 0)
	{
		// block till you get some shit
		if (!clientSocket) return 1;
		if (!clientSocket->HasPendingData(pendingDataSize)) continue;
		buf.Init(0, pendingDataSize);
		clientSocket->Recv(buf.GetData(), buf.Num(), bytesRead);	
		if (bytesRead < 1) {
			UE_LOG(LogTemp, Error, TEXT("Socket did not receive enough data: %d"), bytesRead);
			return 1;
		}
		int32 command = (buf[0] - '0');
		// call custom event with number here
		alexaEvent->Broadcast(command);
	}
	return 0;
}

void FSocketThread::Stop()
{
	StopTaskCounter.Increment();
	socket->Close();
	clientSocket->Close();
}

void FSocketThread::EnsureCompletion()
{
	Stop();
	Thread->WaitForCompletion();
}

void FSocketThread::Shutdown()
{
	if (Runnable)
	{
		Runnable->EnsureCompletion();
		delete Runnable;
		Runnable = NULL;
	}
}
