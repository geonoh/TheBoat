// It's made by Geon Oh


#include "Producer.h"

UProducer* UProducer::Instance = nullptr;

void UProducer::InitInstance(UProducer* NewInstance)
{
	Instance = NewInstance;
}

void UProducer::DestroyInstance()
{
	Instance = nullptr;

	// Free managers
}
