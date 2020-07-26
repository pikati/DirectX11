#include "main.h"
#include "renderer.h"
#include "Field.h"
#include "Transform.h"

Field::Field()
{

}

Field::~Field()
{

}

void Field::Initialize()
{
	gameObject->transform->position = Vector3(0.0f, 0.0f, 0.0f);
	gameObject->transform->rotation = Vector3(0.0f, 0.0f, 0.0f);
	gameObject->transform->scale = Vector3(1.0f, 1.0f, 1.0f);
}

void Field::Update()
{

}

void Field::Draw()
{
}

void Field::Finalize()
{
}