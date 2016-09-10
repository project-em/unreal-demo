// Fill out your copyright notice in the Description page of Project Settings.

#include "PennApps2016F.h"
#include "SocketThread.h"

// Singleton
FSocketThread* FSocketThread::Runnable = NULL;
FSocketThread* FSocketThread::Singleton(FSocket* socket)
{
	//Create new instance of thread if it does not exist
	//		and the platform supports multi threading!
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new FSocketThread(socket);
	}
	return Runnable;
}

FSocketThread::FSocketThread(FSocket* socket)
	: socket(socket), StopTaskCounter(0)
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
	UE_LOG(LogTemp, Warning, TEXT("Prime Number Thread Started!"));
	clientSocket = socket->Accept(TEXT("Connected to client.:"));
	return clinetSocket != NULL;
}

uint32 FSocketThread::Run()
{
	//Initial wait before starting
	FPlatformProcess::Sleep(0.03);
	TArray<uint8> buf;
	int32 bytesRead = 0;
	while (StopTaskCounter.GetValue() == 0)
	{
		// block till you get some shit
		if (!socket) return 1;
		socket.recv(buf.GetData(), buf.Num(), bytesRead);
		if (bytesRead < 2) {
			UE_LOG(LogTemp, Error, TEXT("Socket did not receive enough data."));
			return 1;
		}
		int32 command = buf[0];
		// call custom event with number here
	}
	return 0;
}

void FSocketThread::Stop()
{
	StopTaskCounter.Increment();
	socket->close();
	clientSocket->close();
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