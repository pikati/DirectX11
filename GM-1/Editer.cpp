#include "Editer.h"
#include "Hierarchy.h"
#include "Inspector.h"

void Editer::Initialize()
{
	Hierarchy::Initialize();
	Inspector::Initialize();
}

void Editer::Update()
{
	Hierarchy::Update();
	Inspector::Update();
}

void Editer::Draw()
{
	Hierarchy::Draw();
	Inspector::Draw();
}

void Editer::Finalize()
{
	Inspector::Finalize();
	Hierarchy::Finalize();
}