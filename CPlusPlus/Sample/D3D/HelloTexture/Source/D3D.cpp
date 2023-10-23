#include "D3D.h"

#include <Windows.h>

#include <d3d12.h>
#include <d3dcompiler.h>
#include <dxgi1_6.h>

#pragma comment(lib, "d3d12")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxgi")



Bitmap bitmap;

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
ID3D12DescriptorHeap* srvHeap;
ID3D12PipelineState* pipelineState;
ID3D12GraphicsCommandList* commandList;
SizeType rtvDescriptorSize;

// app resources
ID3D12Resource* vertexBuffer;
D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
ID3D12Resource* texture;

// synchronization objects
SizeType frameIndex;
HANDLE fenceEvent;
ID3D12Fence* fence;
SizeType fenceValue;


struct Vertex
{
	Vector4 position;
	Vector2 uv;
};


Void D3DInit(Void* handle, SizeType width, SizeType height)
{
	{
		auto loaded = File::ReadAllBytes(u8"∞¢ºßµ√“‚.bmp");

		bitmap = Bitmap::Decode(loaded.GetBuffer(), loaded.GetByteCount());
	}

	{
		{
			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			viewport.Width = static_cast<Float32>(width);
			viewport.Height = static_cast<Float32>(height);
			viewport.MinDepth = D3D12_MIN_DEPTH;
			viewport.MaxDepth = D3D12_MAX_DEPTH;
		}
		{
			scissorRect.left = 0;
			scissorRect.right = width;
			scissorRect.top = 0;
			scissorRect.bottom = height;
		}
		swapChain = nullptr;
		device = nullptr;
		Memory::Clear(renderTargets, sizeof(renderTargets));
		commandAllocator = nullptr;
		commandQueue = nullptr;
		rootSignature = nullptr;
		rtvHeap = nullptr;
		srvHeap = nullptr;
		pipelineState = nullptr;
		commandList = nullptr;
		rtvDescriptorSize = 0;

		vertexBuffer = nullptr;
		Memory::Clear(&vertexBufferView, sizeof(vertexBufferView));
		texture = nullptr;

		frameIndex = 0;
		fenceEvent = nullptr;
		fence = nullptr;
		fenceValue = 0;
	}

	IDXGIFactory4* factory;
	CreateDXGIFactory2(0, IID_PPV_ARGS(&factory));

	IDXGIAdapter1* adapter;
	for (SizeType index = 0; factory->EnumAdapters1(index, &adapter) != DXGI_ERROR_NOT_FOUND; index++)break;

	D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));

	{
		D3D12_COMMAND_QUEUE_DESC descriptor;
		Memory::Clear(&descriptor, sizeof(descriptor));
		descriptor.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		descriptor.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		device->CreateCommandQueue(&descriptor, IID_PPV_ARGS(&commandQueue));

		int breakpoint = -1;
	}

	{
		DXGI_SWAP_CHAIN_DESC1 descriptor;
		Memory::Clear(&descriptor, sizeof(descriptor));
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
		Memory::Clear(&descriptor, sizeof(descriptor));
		descriptor.NumDescriptors = FrameCount;
		descriptor.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		descriptor.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		
		device->CreateDescriptorHeap(&descriptor, IID_PPV_ARGS(&rtvHeap));
		rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	{
		D3D12_DESCRIPTOR_HEAP_DESC descriptor;
		Memory::Clear(&descriptor, sizeof(descriptor));
		descriptor.NumDescriptors = 1;
		descriptor.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		descriptor.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

		device->CreateDescriptorHeap(&descriptor, IID_PPV_ARGS(&srvHeap));
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
		D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData;
		Memory::Clear(&featureData, sizeof(featureData));
		featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

		if (device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData)) != S_OK)
			featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;

		D3D12_DESCRIPTOR_RANGE1 ranges[1];
		Memory::Clear(ranges, sizeof(ranges));
		ranges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		ranges[0].NumDescriptors = 1;
		ranges[0].BaseShaderRegister = 0;
		ranges[0].RegisterSpace = 0;
		ranges[0].Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC;
		ranges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		D3D12_ROOT_PARAMETER1 rootParameters[1];
		Memory::Clear(rootParameters, sizeof(rootParameters));
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		rootParameters[0].DescriptorTable.NumDescriptorRanges = 1;
		rootParameters[0].DescriptorTable.pDescriptorRanges = ranges;

		D3D12_STATIC_SAMPLER_DESC sampler;
		Memory::Clear(&sampler, sizeof(sampler));
		sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		sampler.MipLODBias = 0;
		sampler.MaxAnisotropy = 0;
		sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		sampler.MinLOD = 0;
		sampler.MaxLOD = D3D12_FLOAT32_MAX;
		sampler.ShaderRegister = 0;
		sampler.RegisterSpace = 0;
		sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		D3D12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
		Memory::Clear(&rootSignatureDesc, sizeof(rootSignatureDesc));
		rootSignatureDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_1;
		rootSignatureDesc.Desc_1_1.NumParameters = 1;
		rootSignatureDesc.Desc_1_1.pParameters = rootParameters;
		rootSignatureDesc.Desc_1_1.NumStaticSamplers = 1;
		rootSignatureDesc.Desc_1_1.pStaticSamplers = &sampler;
		rootSignatureDesc.Desc_1_1.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	
		ID3DBlob* signature;
		ID3DBlob* error;
		D3D12SerializeVersionedRootSignature(&rootSignatureDesc, &signature, &error);
		device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	}

	{
		ID3DBlob* vertexShader;
		ID3DBlob* pixelShader;

		D3DCompileFromFile(L"Shader.hlsl", nullptr, nullptr, "VS", "vs_5_0", 0, 0, &vertexShader, nullptr);
		D3DCompileFromFile(L"Shader.hlsl", nullptr, nullptr, "PS", "ps_5_0", 0, 0, &pixelShader, nullptr);

		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		D3D12_RASTERIZER_DESC rasterizerState;
		Memory::Clear(&rasterizerState, sizeof(rasterizerState));
		rasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		rasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		rasterizerState.FrontCounterClockwise = false;
		rasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		rasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		rasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		rasterizerState.DepthClipEnable = true;
		rasterizerState.MultisampleEnable = false;
		rasterizerState.AntialiasedLineEnable = false;
		rasterizerState.ForcedSampleCount = 0;
		rasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

		D3D12_BLEND_DESC blendState;
		Memory::Clear(&blendState, sizeof(blendState));
		blendState.AlphaToCoverageEnable = false;
		blendState.IndependentBlendEnable = false;
		{
			D3D12_RENDER_TARGET_BLEND_DESC DefaultValue;
			Memory::Clear(&DefaultValue, sizeof(DefaultValue));

			DefaultValue.BlendEnable = false;
			DefaultValue.LogicOpEnable = false;

			DefaultValue.SrcBlend = D3D12_BLEND_ONE;
			DefaultValue.DestBlend = D3D12_BLEND_ZERO;
			DefaultValue.BlendOp = D3D12_BLEND_OP_ADD;

			DefaultValue.SrcBlendAlpha = D3D12_BLEND_ONE;
			DefaultValue.DestBlendAlpha = D3D12_BLEND_ZERO;
			DefaultValue.BlendOpAlpha = D3D12_BLEND_OP_ADD;

			DefaultValue.LogicOp = D3D12_LOGIC_OP_NOOP;

			DefaultValue.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

			for (SizeType index = 0; index < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; index++)
				blendState.RenderTarget[index] = DefaultValue;
		}

		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
		Memory::Clear(&psoDesc, sizeof(psoDesc));
		psoDesc.InputLayout = { inputElementDescs, 2 };
		psoDesc.pRootSignature = rootSignature;
		psoDesc.VS = { vertexShader->GetBufferPointer(), vertexShader->GetBufferSize()};
		psoDesc.PS = { pixelShader->GetBufferPointer(), pixelShader->GetBufferSize() };
		psoDesc.RasterizerState = rasterizerState;
		psoDesc.BlendState = blendState;
		psoDesc.DepthStencilState.DepthEnable = false;
		psoDesc.DepthStencilState.StencilEnable = false;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.SampleDesc.Count = 1;

		device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState));
	}

	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, pipelineState, IID_PPV_ARGS(&commandList));

	{
		RealType aspect = static_cast<RealType>(width) / height;

		Vertex vertices[] =
		{

			{ { -0.25, 0.25 * aspect, 0, 1 }, { 0, 1 } },
			{ { 0.25, -0.25 * aspect, 0, 1 }, { 1, 0 } },
			{ { -0.25, -0.25 * aspect, 0, 1 }, { 0, 0 } },

			{ { 0.25, -0.25 * aspect, 0, 1 }, { 1, 0 } },
			{ { -0.25, 0.25 * aspect, 0, 1 }, { 0, 1 } },
			{ { 0.25, 0.25 * aspect, 0, 1 }, { 1, 1 } },
		};

		SizeType vertexBufferSize = sizeof(vertices);

		D3D12_HEAP_PROPERTIES heapProperties;
		Memory::Clear(&heapProperties, sizeof(heapProperties));
		heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProperties.CreationNodeMask = 1;
		heapProperties.VisibleNodeMask = 1;

		D3D12_RESOURCE_DESC resourceDesc;
		Memory::Clear(&resourceDesc, sizeof(resourceDesc));
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDesc.Alignment = 0;
		resourceDesc.Width = vertexBufferSize;
		resourceDesc.Height = 1;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = 1;
		resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
		resourceDesc.SampleDesc.Quality = 0;
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		device->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
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
		SizeType imageWidth = bitmap.GetWidth();
		SizeType imageHeight = bitmap.GetHeight();
		SizeType channelCount = bitmap.GetChannelCount();
		SizeType imageSize = imageWidth * imageHeight * channelCount;

		{
			D3D12_RESOURCE_DESC descriptor;
			Memory::Clear(&descriptor, sizeof(descriptor));
			descriptor.MipLevels = 1;
			descriptor.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			descriptor.Width = imageWidth;
			descriptor.Height = imageHeight;
			descriptor.Flags = D3D12_RESOURCE_FLAG_NONE;
			descriptor.DepthOrArraySize = 1;
			descriptor.SampleDesc.Count = 1;
			descriptor.SampleDesc.Quality = 0;
			descriptor.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

			D3D12_HEAP_PROPERTIES heapProperties;
			Memory::Clear(&heapProperties, sizeof(heapProperties));
			heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
			heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			heapProperties.CreationNodeMask = 1;
			heapProperties.VisibleNodeMask = 1;

			device->CreateCommittedResource(
				&heapProperties,
				D3D12_HEAP_FLAG_NONE,
				&descriptor,
				D3D12_RESOURCE_STATE_COPY_DEST,
				nullptr,
				IID_PPV_ARGS(&texture)
			);
		}

		ID3D12Resource* textureUploadHeap;
		{
			D3D12_RESOURCE_DESC descriptor;
			Memory::Clear(&descriptor, sizeof(descriptor));
			descriptor.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			descriptor.Alignment = 0;
			descriptor.Width = imageSize;
			descriptor.Height = 1;
			descriptor.DepthOrArraySize = 1;
			descriptor.MipLevels = 1;
			descriptor.Format = DXGI_FORMAT_UNKNOWN;
			descriptor.SampleDesc.Count = 1;
			descriptor.SampleDesc.Quality = 0;
			descriptor.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			descriptor.Flags = D3D12_RESOURCE_FLAG_NONE;

			D3D12_HEAP_PROPERTIES heapProperties;
			Memory::Clear(&heapProperties, sizeof(heapProperties));
			heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
			heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			heapProperties.CreationNodeMask = 1;
			heapProperties.VisibleNodeMask = 1;

			device->CreateCommittedResource(
				&heapProperties,
				D3D12_HEAP_FLAG_NONE,
				&descriptor,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&textureUploadHeap)
			);

			D3D12_SUBRESOURCE_DATA textureData;
			Memory::Clear(&textureData, sizeof(textureData));
			textureData.pData = bitmap.GetBuffer();
			textureData.RowPitch = imageWidth * channelCount;
			textureData.SlicePitch = imageSize;

			D3D12_PLACED_SUBRESOURCE_FOOTPRINT layout;
			Memory::Clear(&layout, sizeof(layout));
			layout.Offset = 0;
			layout.Footprint.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			layout.Footprint.Width = imageWidth;
			layout.Footprint.Height = imageHeight;
			layout.Footprint.Depth = 1;
			layout.Footprint.RowPitch = imageWidth * channelCount;

			{
				Void* textureUploadHeapDataBegin;
				textureUploadHeap->Map(0, nullptr, &textureUploadHeapDataBegin);
				Memory::Copy(bitmap.GetBuffer(), textureUploadHeapDataBegin, imageSize);
				textureUploadHeap->Unmap(0, nullptr);
			}

			{
				D3D12_TEXTURE_COPY_LOCATION source;
				D3D12_TEXTURE_COPY_LOCATION destination;

				Memory::Clear(&source, sizeof(source));
				source.pResource = textureUploadHeap;
				source.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
				source.PlacedFootprint = layout;

				Memory::Clear(&destination, sizeof(destination));
				destination.pResource = texture;
				destination.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
				destination.SubresourceIndex = 0;

				commandList->CopyTextureRegion(&destination, 0, 0, 0, &source, nullptr);
			}

			{
				D3D12_RESOURCE_BARRIER barrier;
				Memory::Clear(&barrier, sizeof(barrier));
				barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
				barrier.Transition.pResource = texture;
				barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
				barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
				barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

				commandList->ResourceBarrier(1, &barrier);
			}

			{
				D3D12_SHADER_RESOURCE_VIEW_DESC descriptor;
				Memory::Clear(&descriptor, sizeof(descriptor));
				descriptor.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
				descriptor.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
				descriptor.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
				descriptor.Texture2D.MipLevels = 1;

				device->CreateShaderResourceView(texture, &descriptor, srvHeap->GetCPUDescriptorHandleForHeapStart());
			}
		}
	}

	commandList->Close();
	ID3D12CommandList* commandLists[] = { commandList };
	commandQueue->ExecuteCommandLists(1, commandLists);

	{
		device->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
		fenceValue++;

		fenceEvent = CreateEvent(nullptr, false, false, nullptr);

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

	ID3D12DescriptorHeap* descrptorHeaps[] = { srvHeap };
	commandList->SetDescriptorHeaps(1, descrptorHeaps);

	commandList->SetGraphicsRootDescriptorTable(0, srvHeap->GetGPUDescriptorHandleForHeapStart());
	commandList->RSSetViewports(1, &viewport);
	commandList->RSSetScissorRects(1, &scissorRect);

	{
		D3D12_RESOURCE_BARRIER barrier;
		Memory::Clear(&barrier, sizeof(barrier));
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = renderTargets[frameIndex];
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		commandList->ResourceBarrier(1, &barrier);
	}

	{
		auto rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += rtvDescriptorSize * frameIndex;

		commandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

		Color clearColor = Color_White * 0.1;
		commandList->ClearRenderTargetView(rtvHandle, reinterpret_cast<const Float32*>(&clearColor), 0, nullptr);
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		commandList->IASetVertexBuffers(0, 1, &vertexBufferView);
		commandList->DrawInstanced(6, 1, 0, 0);
	}

	{
		D3D12_RESOURCE_BARRIER barrier;
		Memory::Clear(&barrier, sizeof(barrier));
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = renderTargets[frameIndex];
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		commandList->ResourceBarrier(1, &barrier);
	}

	commandList->Close();
}

Void D3DWait()
{
	auto temporary = fenceValue;
	commandQueue->Signal(fence, temporary);

	if (fence->GetCompletedValue() < temporary)
	{
		fence->SetEventOnCompletion(temporary, fenceEvent);
		WaitForSingleObject(fenceEvent, INFINITE);
	}

	frameIndex = swapChain->GetCurrentBackBufferIndex();
}