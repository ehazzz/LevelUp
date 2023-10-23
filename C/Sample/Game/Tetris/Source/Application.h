#pragma once

#include "Runtime.h"

#include "Framework.h"



Void OnAppInit(Void* userParam);
Void OnAppResize(SizeType width, SizeType height);
Void OnAppFocus(Bool isFocused);
Void OnAppQuit(Bool wantToQuit);

Void OnUpdate(RealType deltaTime);
Void OnRender(Void* RenderTarget);