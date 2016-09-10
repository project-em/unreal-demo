// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Networking.h"

/**
 * 
 */
class PENNAPPS2016F_API FSocketThread : public FRunnable
{
	/** Singleton instance, can access the thread any time via static accessor, if it is active! */
	static  FSocketThread* Runnable;

	/** Thread to run the worker FRunnable on */
	FRunnableThread* Thread;

	/** Stop this thread? Uses Thread Safe Counter */
	FThreadSafeCounter StopTaskCounter;

	FSocket *socket;
	FSocket *clientSocket;
public:
	//~~~ Thread Core Functions ~~~

	//Constructor / Destructor
	FSocketThread(FSocket* socket);
	virtual ~FSocketThread();

	// Begin FRunnable interface.
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();
	// End FRunnable interface

	/** Makes sure this thread has stopped properly */
	void EnsureCompletion();

	// Singleton
	static FSocketThread* Singleton(FSocket* socket);
	static void Shutdown();
	static bool IsThreadFinished();

};