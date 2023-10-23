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
D3D12_VIEWPORT viewport;
D3D12_RECT scissorRect;
IDXGISwapChain3* swapChain;
ID3D12Device* device;
ID3D12Resource* renderTargets[FrameCount];
ID3D12CommandAllocator* commandAllocator;
ID3D12CommandQueue* commandQueue;
ID3D12RootSignature* rootSignature;
ID3D12DescriptorHeap* rtvHeap;
ID3D12PipelineState* pipelineState;
ID3D12GraphicsCommandList* commandList;
SizeType rtvDescriptorSize;

// app resources
ID3D12Resource* vertexBuffer;
D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

// Synchronization objects
SizeType frameIndex;
HANDLE fenceEvent;
ID3D12Fence* fence;
SizeType fenceValue;


struct Vertex
{
	Vector4 position;
	Color color;
};


static Void Initialize()
{
	Memory::Clear(&viewport, sizeof(D3D12_VIEWPORT));
	Memory::Clear(&scissorRect, sizeof(D3D12_RECT));
	swapChain = nullptr;
	device = nullptr;
	Memory::Clear(renderTargets, sizeof(renderTargets));
	commandAllocator = nullptr;
	commandQueue = nullptr;
	rootSignature = nullptr;
	rtvHeap = nullptr;
	pipelineState = nullptr;
	commandList = nullptr;
	rtvDescriptorSize = 0;

	vertexBuffer = nullptr;
	Memory::Clear(&vertexBuffer, sizeof(D3D12_VERTEX_BUFFER_VIEW));

	frameIndex = 0;
	fenceEvent = nullptr;
	fence = nullptr;
	fenceValue = 0;
}


Void D3DInit(Void* handle, SizeType width, SizeType height)
{
	{
		Initialize();

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<Float32>(width);
		viewport.Height = static_cast<Float32>(height);
		viewport.MinDepth = D3D12_MIN_DEPTH;
		viewport.MaxDepth = D3D12_MAX_DEPTH;

		scissorRect.left = 0;
		scissorRect.right = width;
		scissorRect.top = 0;
		scissorRect.bottom = height;
	}

	IDXGIFactory4* factory;
	CreateDXGIFactory2(0, IID_PPV_ARGS(&factory));

	IDXGIAdapter1* adapter;
	for (SizeType index = 0; factory->EnumAdapters1(index, &adapter) != DXGI_ERROR_NOT_FOUND; index++) break;

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

	{
		D3D12_ROOT_SIGNATURE_DESC descriptor;
		Memory::Clear(&descriptor, sizeof(D3D12_ROOT_SIGNATURE_DESC));
		descriptor.NumParameters = 0;
		descriptor.pParameters = nullptr;
		descriptor.NumStaticSamplers = 0;
		descriptor.pStaticSamplers = nullptr;
		descriptor.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		ID3DBlob* signature;
		ID3DBlob* error;

		D3D12SerializeRootSignature(&descriptor, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
		device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	}

	{
		ID3DBlob* vertexShader;
		ID3DBlob* pixelShader;

		D3DCompileFromFile(L"Shader.hlsl", nullptr, nullptr, "VS", "vs_5_0", 0, 0, &vertexShader, nullptr);
		D3DCompileFromFile(L"Shader.hlsl", nullptr, nullptr, "PS", "ps_5_0", 0, 0, &pixelShader, nullptr);

		D3D12_INPUT_ELEMENT_DESC inputElements[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		D3D12_RASTERIZER_DESC rasterizerState;
		Memory::Clear(&rasterizerState, sizeof(D3D12_RASTERIZER_DESC));
		rasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		rasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		rasterizerState.FrontCounterClockwise = false;
		rasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		rasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		rasterizerState.DepthClipEnable = true;
		rasterizerState.MultisampleEnable = false;
		rasterizerState.AntialiasedLineEnable = false;
		rasterizerState.ForcedSampleCount = 0;
		rasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

		D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendState;
		Memory::Clear(&defaultRenderTargetBlendState, sizeof(D3D12_RENDER_TARGET_BLEND_DESC));
		defaultRenderTargetBlendState.BlendEnable = false;
		defaultRenderTargetBlendState.LogicOpEnable = false;
		defaultRenderTargetBlendState.SrcBlend = D3D12_BLEND_ONE;
		defaultRenderTargetBlendState.DestBlend = D3D12_BLEND_ZERO;
		defaultRenderTargetBlendState.BlendOp = D3D12_BLEND_OP_ADD;
		defaultRenderTargetBlendState.SrcBlendAlpha = D3D12_BLEND_ONE;
		defaultRenderTargetBlendState.DestBlendAlpha = D3D12_BLEND_ZERO;
		defaultRenderTargetBlendState.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		defaultRenderTargetBlendState.LogicOp = D3D12_LOGIC_OP_NOOP;
		defaultRenderTargetBlendState.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

		D3D12_BLEND_DESC blendState;
		Memory::Clear(&blendState, sizeof(D3D12_BLEND_DESC));
		blendState.AlphaToCoverageEnable = false;
		blendState.IndependentBlendEnable = false;
		for (SizeType index = 0; index < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; index++) blendState.RenderTarget[index] = defaultRenderTargetBlendState;

		D3D12_GRAPHICS_PIPELINE_STATE_DESC pso;
		Memory::Clear(&pso, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
		pso.InputLayout = { inputElements, 2 };
		pso.pRootSignature = rootSignature;
		pso.VS = { vertexShader->GetBufferPointer(), vertexShader->GetBufferSize() };
		pso.PS = { pixelShader->GetBufferPointer(), pixelShader->GetBufferSize() };
		pso.RasterizerState = rasterizerState;
		pso.BlendState = blendState;
		pso.DepthStencilState.DepthEnable = false;
		pso.DepthStencilState.StencilEnable = false;
		pso.SampleMask = UINT_MAX;
		pso.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pso.NumRenderTargets = 1;
		pso.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		pso.SampleDesc.Count = 1;
		
		device->CreateGraphicsPipelineState(&pso, IID_PPV_ARGS(&pipelineState));

		int breakpoint = -1;
	}

	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, pipelineState, IID_PPV_ARGS(&commandList));
	
	commandList->Close();

	{
		const RealType aspect = static_cast<RealType>(width) / height;

		Vertex vertices[] =
		{
			{ { 0, 0.25 * aspect, 0, 1 }, Color_Red },
			{ { 0.25, -0.25 * aspect, 0, 1}, Color_Green },
			{ { -0.25, -0.25 * aspect, 0, 1}, Color_Blue },
		};

		SizeType vertexBufferSize = sizeof(vertices);

		D3D12_HEAP_PROPERTIES heapProperties;
		Memory::Clear(&heapProperties, sizeof(D3D12_HEAP_PROPERTIES));
		heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProperties.CreationNodeMask = 1;
		heapProperties.VisibleNodeMask = 1;

		D3D12_RESOURCE_DESC resourceDescriptor;
		Memory::Clear(&resourceDescriptor, sizeof(D3D12_RESOURCE_DESC));
		resourceDescriptor.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDescriptor.Alignment = 0;
		resourceDescriptor.Width = vertexBufferSize;
		resourceDescriptor.Height = 1;
		resourceDescriptor.DepthOrArraySize = 1;
		resourceDescriptor.MipLevels = 1;
		resourceDescriptor.Format = DXGI_FORMAT_UNKNOWN;
		resourceDescriptor.SampleDesc.Count = 1;
		resourceDescriptor.SampleDesc.Quality = 0;
		resourceDescriptor.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		resourceDescriptor.Flags = D3D12_RESOURCE_FLAG_NONE;

		device->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resourceDescriptor,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&vertexBuffer)
		);

		Void* vertexDataBegin;
		D3D12_RANGE readRange = { 0, 0 };
		vertexBuffer->Map(0, &readRange, &vertexDataBegin);
		Memory::Copy(vertices, vertexDataBegin, vertexBufferSize);
		vertexBuffer->Unmap(0, nullptr);

		vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
		vertexBufferView.StrideInBytes = sizeof(Vertex);
		vertexBufferView.SizeInBytes = vertexBufferSize;
	}

	{
		device->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
		fenceValue++;

		fenceEvent = CreateEventW(nullptr, false, false, nullptr);

		D3DWait();
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

	commandList->SetGraphicsRootSignature(rootSignature);
	commandList->RSSetViewports(1, &viewport);
	commandList->RSSetScissorRects(1, &scissorRect);

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

	commandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

	{
		commandList->ClearRenderTargetView(rtvHandle, reinterpret_cast<const Float32*>(&Color_Black), 0, nullptr);
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		commandList->IASetVertexBuffers(0, 1, &vertexBufferView);
		commandList->DrawInstanced(3, 1, 0, 0);
	}

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