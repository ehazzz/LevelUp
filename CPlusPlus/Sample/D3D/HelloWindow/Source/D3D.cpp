#include "D3D.h"

#include <Windows.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3dcompiler")



constexpr SizeType FrameCount = 2;

// pipeline objects
IDXGISwapChain3* swapChain;
ID3D12Device* device;
ID3D12Resource* renderTargets[FrameCount];
ID3D12CommandAllocator* commandAllocator;
ID3D12CommandQueue* commandQueue;
ID3D12DescriptorHeap* rtvHeap;
ID3D12PipelineState* pipelineState;
ID3D12GraphicsCommandList* commandList;
SizeType rtvDescriptorSize;

// synchronization objects
SizeType frameIndex;
HANDLE fenceEvent;
ID3D12Fence* fence;
SizeType fenceValue;


static Void Initialize()
{
	swapChain = nullptr;
	device = nullptr;
	Memory::Clear(renderTargets, sizeof(renderTargets));
	commandAllocator = nullptr;
	commandQueue = nullptr;
	rtvHeap = nullptr;
	pipelineState = nullptr;
	commandList = nullptr;
	rtvDescriptorSize = 0;

	frameIndex = 0;
	fenceEvent = nullptr;
	fence = nullptr;
	fenceValue = 0;
}

static Void Release()
{
	swapChain->Release();
	device->Release();
	for (SizeType index = 0; index < FrameCount; index++) renderTargets[index]->Release();
	commandAllocator->Release();
	commandQueue->Release();
	rtvHeap->Release();
	pipelineState->Release();
	commandList->Release();

	CloseHandle(fenceEvent);
	fence->Release();
}


Void D3DInit(Void* handle, SizeType width, SizeType height)
{
	Initialize();

	IDXGIFactory4* factory;
	CreateDXGIFactory2(0, IID_PPV_ARGS(&factory));

	IDXGIAdapter1* adapter;
	for (SizeType index = 0; factory->EnumAdapters1(index, &adapter) != DXGI_ERROR_NOT_FOUND; index++)break;

	D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));

	{
		D3D12_COMMAND_QUEUE_DESC descriptor;
		Memory::Clear(&descriptor, sizeof(D3D12_COMMAND_QUEUE_DESC));
		descriptor.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		descriptor.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		device->CreateCommandQueue(&descriptor, IID_PPV_ARGS(&commandQueue));
	}

	{
		DXGI_SWAP_CHAIN_DESC1 descriptor;
		Memory::Clear(&descriptor, sizeof(DXGI_SWAP_CHAIN_DESC1));
		descriptor.BufferCount = FrameCount;
		descriptor.Width = width;
		descriptor.Height = height;
		descriptor.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		descriptor.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		descriptor.SampleDesc.Count = 1;

		IDXGISwapChain1* swapChain1;
		factory->CreateSwapChainForHwnd(
			commandQueue,
			reinterpret_cast<HWND>(handle),
			&descriptor,
			nullptr,
			nullptr,
			&swapChain1
		);

		swapChain = reinterpret_cast<IDXGISwapChain3*>(swapChain1);
		frameIndex = swapChain->GetCurrentBackBufferIndex();
	}

	{
		D3D12_DESCRIPTOR_HEAP_DESC descriptor;
		Memory::Clear(&descriptor, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
		descriptor.NumDescriptors = FrameCount;
		descriptor.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		descriptor.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		
		device->CreateDescriptorHeap(&descriptor, IID_PPV_ARGS(&rtvHeap));
		rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	{
		auto rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();

		for (SizeType index = 0; index < FrameCount; index++)
		{
			swapChain->GetBuffer(index, IID_PPV_ARGS(renderTargets + index));
			device->CreateRenderTargetView(renderTargets[index], nullptr, rtvHandle);

			rtvHandle.ptr += rtvDescriptorSize;
		}
	}

	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
	
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, pipelineState, IID_PPV_ARGS(&commandList));
	commandList->Close();

	{
		device->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
		fenceValue++;

		fenceEvent = CreateEventW(nullptr, false, false, nullptr);
	}
}

Void D3DUpdate()
{

}

Void D3DRender()
{
	D3DExecute();

	ID3D12CommandList* commandLists[] = { commandList };
	commandQueue->ExecuteCommandLists(1, commandLists);

	swapChain->Present(1, 0);

	D3DWait();
}

Void D3DDestroy()
{
	D3DWait();

	CloseHandle(fenceEvent);
}

Void D3DExecute()
{
	commandAllocator->Reset();
	commandList->Reset(commandAllocator, pipelineState);

	{
		D3D12_RESOURCE_BARRIER descriptor;
		Memory::Clear(&descriptor, sizeof(D3D12_RESOURCE_BARRIER));
		descriptor.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		descriptor.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		descriptor.Transition.pResource = renderTargets[frameIndex];
		descriptor.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		descriptor.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		descriptor.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		commandList->ResourceBarrier(1, &descriptor);
	}

	auto rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += rtvDescriptorSize * frameIndex;

	commandList->ClearRenderTargetView(rtvHandle, reinterpret_cast<const Float32*>(&Color_Red), 0, nullptr);

	{
		D3D12_RESOURCE_BARRIER descriptor;
		Memory::Clear(&descriptor, sizeof(D3D12_RESOURCE_BARRIER));
		descriptor.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		descriptor.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		descriptor.Transition.pResource = renderTargets[frameIndex];
		descriptor.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		descriptor.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		descriptor.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		commandList->ResourceBarrier(1, &descriptor);
	}

	commandList->Close();
}

Void D3DWait()
{
	auto temporary = fenceValue;
	commandQueue->Signal(fence, temporary);
	fenceValue++;

	if (fence->GetCompletedValue() < temporary)
	{
		fence->SetEventOnCompletion(temporary, fenceEvent);
		WaitForSingleObject(fenceEvent, INFINITE);
	}

	frameIndex = swapChain->GetCurrentBackBufferIndex();
}
