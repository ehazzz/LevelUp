#pragma once

#include "Runtime.h"



Void OnAppInit(Void* userParameter);
Void OnAppResize(SizeType width, SizeType height);
Void OnAppFocused(Bool isFocused);
Void OnAppQuit(Bool wantToQuit);

Void OnUpdate(RealType deltaTime);
Void OnRender(Void* RenderTarget);