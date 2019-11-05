// Copyright 2019 Denis Maximenko


#include "PT_Structs.h"

#include "UObject/ConstructorHelpers.h"

FPT_BikeModel::FPT_BikeModel() : Name(NAME_None), Mesh(nullptr), AbilityComponentClass(nullptr)
{
  static ConstructorHelpers::FObjectFinder<UStaticMesh> BikeMesh(TEXT("/Game/Assets/Meshes/Sphere.Sphere"));
  if (BikeMesh.Succeeded())
  {
    Mesh = BikeMesh.Object;
  }
}
