
#include "Precomp.h"
#include "VulkanRenderDevice.h"
#include <zwidget/core/widget.h>
#include <zvulkan/vulkaninstance.h>
#include <zvulkan/vulkandevice.h>
#include <zvulkan/vulkansurface.h>
#include <zvulkan/vulkancompatibledevice.h>
#include <zvulkan/vulkanobjects.h>
#include <zvulkan/vulkanswapchain.h>
#include <zvulkan/vulkanbuilders.h>

VulkanRenderDevice::VulkanRenderDevice(Widget* viewport) : viewport(viewport)
{
	instance = VulkanInstanceBuilder()
		.RequireSurfaceExtensions()
		.DebugLayer(false)
		.Create();

	surface = VulkanSurfaceBuilder()
		.Win32Window((HWND)viewport->GetNativeHandle())
		.Create(instance);

	device = VulkanDeviceBuilder()
		.Surface(surface)
		.Create(instance);

	swapchain = VulkanSwapChainBuilder()
		.Create(device.get());

	commandPool = CommandPoolBuilder()
		.DebugName("commandPool")
		.Create(device.get());

	imageAvailableSemaphore = SemaphoreBuilder()
		.DebugName("imageAvailableSemaphore")
		.Create(device.get());

	renderFinishedSemaphore = SemaphoreBuilder()
		.DebugName("renderFinishedSemaphore")
		.Create(device.get());

	presentFinishedFence = FenceBuilder()
		.DebugName("presentFinishedFence")
		.Create(device.get());

	// GLSL for a vertex shader

	std::string includedCode = R"(
			layout(set = 0, binding = 0, std140) uniform Uniforms
			{
				mat4 ProjectionMatrix;
				mat4 ViewMatrix;
				vec2 ViewportOrigin;
				vec2 ViewportScale;
			};
		)";

	std::string versionBlock = R"(
			#version 450
			#extension GL_GOOGLE_include_directive : enable
		)";

	std::string vertexCode = R"(

			#include "uniforms.glsl"

			layout(location = 0) in vec4 aPosition;
			layout(location = 1) in vec3 aTexCoord;
			layout(location = 2) in vec4 aColor;

			layout(location = 0) out vec3 texCoord;
			layout(location = 1) out vec4 color;

			void main()
			{
				texCoord = aTexCoord;
				color = aColor;
				gl_Position = ProjectionMatrix * ViewMatrix * aPosition;
			}
		)";

	// GLSL for two fragment shaders. One textured and one without

	std::string fragmentShaderNoTexCode = R"(

			layout(location = 0) in vec3 texCoord;
			layout(location = 1) in vec4 color;
			layout(location = 0) out vec4 outColor;

			void main()
			{
				outColor = color;
			}
		)";

	std::string fragmentShaderTexturedCode = R"(

			layout(set = 1, binding = 0) uniform sampler2D Texture;

			layout(location = 0) in vec3 texCoord;
			layout(location = 1) in vec4 color;
			layout(location = 0) out vec4 outColor;

			void main()
			{
				outColor = texture(Texture, texCoord.st) * color;
				// if (outColor.a < 0.5) discard;
			}
		)";

	std::string fragmentShaderLineCode = R"(

			#include "uniforms.glsl"

			layout(location = 0) in vec3 texCoord;
			layout(location = 1) in vec4 color;
			layout(location = 0) out vec4 outColor;

			void main()
			{
				outColor = color;

				vec2 fragpos = (gl_FragCoord.xy - ViewportOrigin) / ViewportScale;
				vec2 pixelpos = floor(fragpos) + 0.5;
				float dist = dot(vec3(pixelpos, 1.0), texCoord);
				outColor.a *= clamp(1.0 - abs(dist), 0.0, 1.0);
			}
		)";

	// Create a vertex shader

	auto oninclude = [=](auto headerName, auto includerName, size_t depth) {
		if (headerName == "uniforms.glsl")
			return ShaderIncludeResult(headerName, includedCode);
		else
			return ShaderIncludeResult("File not found: " + headerName);
		};

	vertexShader = ShaderBuilder()
		.Type(ShaderType::Vertex)
		.AddSource("versionblock", versionBlock)
		.AddSource("vertexCode.glsl", vertexCode)
		.OnIncludeLocal(oninclude)
		.DebugName("vertexShader")
		.Create("vertex", device.get());

	// Create fragment shaders

	fragmentShaderNoTex = ShaderBuilder()
		.Type(ShaderType::Fragment)
		.AddSource("versionblock", versionBlock)
		.AddSource("fragmentShaderNoTex.glsl", fragmentShaderNoTexCode)
		.OnIncludeLocal(oninclude)
		.DebugName("fragmentShaderNoTex")
		.Create("fragmentShaderNoTex", device.get());

	fragmentShaderTextured = ShaderBuilder()
		.Type(ShaderType::Fragment)
		.AddSource("versionblock", versionBlock)
		.AddSource("fragmentShaderTextured.glsl", fragmentShaderTexturedCode)
		.OnIncludeLocal(oninclude)
		.DebugName("fragmentShaderTextured")
		.Create("fragmentShaderTextured", device.get());

	fragmentShaderLine = ShaderBuilder()
		.Type(ShaderType::Fragment)
		.AddSource("versionblock", versionBlock)
		.AddSource("fragmentShaderLine.glsl", fragmentShaderLineCode)
		.OnIncludeLocal(oninclude)
		.DebugName("fragmentShaderLine")
		.Create("fragmentShaderNoTex", device.get());

	// Create descriptor set layouts

	uniformSetLayout = DescriptorSetLayoutBuilder()
		.AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT)
		.DebugName("uniformSetLayout")
		.Create(device.get());

	textureSetLayout = DescriptorSetLayoutBuilder()
		.AddBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT)
		.DebugName("textureSetLayout")
		.Create(device.get());

	// Create a pipeline layouts

	pipelineLayoutNoTex = PipelineLayoutBuilder()
		.AddSetLayout(uniformSetLayout.get())
		.DebugName("pipelineLayoutNoTex")
		.Create(device.get());

	pipelineLayoutTextured = PipelineLayoutBuilder()
		.AddSetLayout(uniformSetLayout.get())
		.AddSetLayout(textureSetLayout.get())
		.DebugName("pipelineLayoutTextured")
		.Create(device.get());

	// Create a render pass where we clear the frame buffer when it begins and ends the pass by transitioning the image to what the swap chain needs to present it

	renderPass = RenderPassBuilder()
		.AddAttachment(VK_FORMAT_B8G8R8A8_UNORM, VK_SAMPLE_COUNT_1_BIT, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
		.AddSubpass()
		.AddSubpassColorAttachmentRef(0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
		.Create(device.get());

	// Create pipelines (which shaders to use, blending rules, etc.)

	pipelineNoTex = GraphicsPipelineBuilder()
		.RenderPass(renderPass.get())
		.Layout(pipelineLayoutNoTex.get())
		.AddVertexBufferBinding(0, sizeof(Vertex))
		.AddVertexAttribute(0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, x))
		.AddVertexAttribute(1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, s))
		.AddVertexAttribute(2, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Vertex, r))
		.AddVertexShader(vertexShader.get())
		.AddFragmentShader(fragmentShaderNoTex.get())
		.AddDynamicState(VK_DYNAMIC_STATE_VIEWPORT)
		.AddDynamicState(VK_DYNAMIC_STATE_SCISSOR)
		.AddColorBlendAttachment(ColorBlendAttachmentBuilder().AlphaBlendMode().Create())
		.DebugName("pipelineNoTex")
		.Create(device.get());

	pipelineLine = GraphicsPipelineBuilder()
		.RenderPass(renderPass.get())
		.Layout(pipelineLayoutNoTex.get())
		.AddVertexBufferBinding(0, sizeof(Vertex))
		.AddVertexAttribute(0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, x))
		.AddVertexAttribute(1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, s))
		.AddVertexAttribute(2, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Vertex, r))
		.AddVertexShader(vertexShader.get())
		.AddFragmentShader(fragmentShaderLine.get())
		.AddDynamicState(VK_DYNAMIC_STATE_VIEWPORT)
		.AddDynamicState(VK_DYNAMIC_STATE_SCISSOR)
		.AddColorBlendAttachment(ColorBlendAttachmentBuilder().AlphaBlendMode().Create())
		.DebugName("pipelineLine")
		.Create(device.get());

	pipelineTextured = GraphicsPipelineBuilder()
		.RenderPass(renderPass.get())
		.Layout(pipelineLayoutTextured.get())
		.AddVertexBufferBinding(0, sizeof(Vertex))
		.AddVertexAttribute(0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, x))
		.AddVertexAttribute(1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, s))
		.AddVertexAttribute(2, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Vertex, r))
		.AddVertexShader(vertexShader.get())
		.AddFragmentShader(fragmentShaderTextured.get())
		.AddDynamicState(VK_DYNAMIC_STATE_VIEWPORT)
		.AddDynamicState(VK_DYNAMIC_STATE_SCISSOR)
		.AddColorBlendAttachment(ColorBlendAttachmentBuilder().AlphaBlendMode().Create())
		.DebugName("pipelineTextured")
		.Create(device.get());

	// Create a persistently mapped vertex buffer

	vertexBuffer = BufferBuilder()
		.Usage(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VMA_MEMORY_USAGE_UNKNOWN, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT)
		.MemoryType(
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
		.Size(maxVertices)
		.DebugName("vertexBuffer")
		.Create(device.get());

	vertices = (Vertex*)vertexBuffer->Map(0, sizeof(Vertex) * 6);

	// Create a persistently mapped uniform buffer

	uniformsBuffer = BufferBuilder()
		.Usage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_UNKNOWN, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT)
		.MemoryType(
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
		.Size(sizeof(Uniforms))
		.DebugName("uniformsBuffer")
		.Create(device.get());

	uniforms = (Uniforms*)uniformsBuffer->Map(0, sizeof(Uniforms));

	// Create persistently mapped staging buffer used for texture uploads

	uploadBuffer = BufferBuilder()
		.Usage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_UNKNOWN, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT)
		.MemoryType(
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
		.Size(uploadBufferSize)
		.DebugName("uploadBuffer")
		.Create(device.get());

	uploads = (uint8_t*)uploadBuffer->Map(0, uploadBufferSize);

	// Create sampler object

	sampler = SamplerBuilder()
		.MinFilter(VK_FILTER_NEAREST)
		.MagFilter(VK_FILTER_NEAREST)
		.DebugName("sampler")
		.Create(device.get());

	// Create descriptor set for the uniform buffer

	uniformSetPool = DescriptorPoolBuilder()
		.AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1)
		.MaxSets(1)
		.DebugName("uniformBufferPool")
		.Create(device.get());

	uniformSet = uniformSetPool->allocate(uniformSetLayout.get());

	WriteDescriptors()
		.AddBuffer(uniformSet.get(), 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, uniformsBuffer.get())
		.Execute(device.get());
}

VulkanRenderDevice::~VulkanRenderDevice()
{
	vkDeviceWaitIdle(device->device);

	while (!cachedTextures.empty())
		DestroyCachedTexture(cachedTextures.back());

	vertexBuffer->Unmap();
	uniformsBuffer->Unmap();
	uploadBuffer->Unmap();
}

bool VulkanRenderDevice::Begin()
{
	// How big is the window client area in this frame?
	RECT clientRect = {};
	GetClientRect((HWND)viewport->GetNativeHandle(), &clientRect);
	width = clientRect.right;
	height = clientRect.bottom;

	// Do we need to resize or recreate the swapchain?
	if (width > 0 && height > 0 && width != lastWidth || height != lastHeight || swapchain->Lost())
	{
		lastWidth = width;
		lastHeight = height;
		framebuffers.clear();

		swapchain->Create(width, height, 1, true, false, false);

		// Create frame buffer objects for the new swap chain images
		for (int imageIndex = 0; imageIndex < swapchain->ImageCount(); imageIndex++)
		{
			framebuffers.push_back(FramebufferBuilder()
				.RenderPass(renderPass.get())
				.AddAttachment(swapchain->GetImageView(imageIndex))
				.Size(width, height)
				.DebugName("framebuffer")
				.Create(device.get()));
		}
	}

	// Try acquire a frame buffer image from the swap chain
	imageIndex = swapchain->AcquireImage(imageAvailableSemaphore.get());
	if (imageIndex == -1)
		return false;

	// Create a command buffer and begin adding commands to it
	drawcommands = commandPool->createBuffer();
	drawcommands->begin();

	// Begin a render pass
	RenderPassBegin()
		.RenderPass(renderPass.get())
		.Framebuffer(framebuffers[imageIndex].get())
		.AddClearColor(0.0f, 0.0f, 0.0f, 1.0f)
		.AddClearDepthStencil(1.0f, 0)
		.RenderArea(0, 0, width, height)
		.Execute(drawcommands.get());

	// Set the viewport to cover the window
	VkViewport viewport = {};
	viewport.width = (float)width;
	viewport.height = (float)height;
	viewport.maxDepth = 1.0f;
	drawcommands->setViewport(0, 1, &viewport);

	// Bind the vertex buffer
	VkDeviceSize offset = 0;
	drawcommands->bindVertexBuffers(0, 1, &vertexBuffer->buffer, &offset);

	// Bind the uniform buffer once (share it between the pipelines)
	drawcommands->bindDescriptorSet(VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayoutNoTex.get(), 0, uniformSet.get());

	int letterboxwidth = 320 * height / 240;

	float viewMatrix[16] =
	{
		letterboxwidth / 320.0f, 0.0f, 0.0f, 0.0f,
		0.0f, height / 200.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		(width - letterboxwidth) * 0.5f, 0.0f, 0.0f, 1.0f
	};
	float projMatrix[16] =
	{
		2.0f / width, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f / height, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 1.0f
	};

	memcpy(uniforms->ProjectionMatrix, projMatrix, sizeof(float) * 16);
	memcpy(uniforms->ViewMatrix, viewMatrix, sizeof(float) * 16);

	uniforms->ViewportX = std::round((width - letterboxwidth) * 0.5f);
	uniforms->ViewportY = 0.0f;
	uniforms->ViewportScaleX = letterboxwidth / 320.0f;
	uniforms->ViewportScaleY = height / 200.0f;

	// Set the scissor box
	VkRect2D scissor = {};
	scissor.offset.x = (int)std::round((width - letterboxwidth) * 0.5f);
	scissor.extent.width = letterboxwidth;
	scissor.extent.height = height;
	drawcommands->setScissor(0, 1, &scissor);

	vertexPos = 0;

	return true;
}

void VulkanRenderDevice::DrawLine(int x1, int y1, int x2, int y2, float r, float g, float b, float a)
{
	if (x1 == x2 && y1 == y2)
		return;

	vec2 start(x1 + 0.5f, y1 + 0.5f);
	vec2 end(x2 + 0.5f, y2 + 0.5f);
	vec2 dir = normalize(end - start);
	vec2 normal(-dir.y, dir.x);
	vec2 normal2 = normal * 2.0f;
	vec2 v0 = start - normal2 - dir * 0.5f;
	vec2 v1 = start + normal2 - dir * 0.5f;
	vec2 v2 = end + normal2 + dir * 0.5f;
	vec2 v3 = end - normal2 + dir * 0.5f;

	float aa = normal.x;
	float bb = normal.y;
	float cc = -dot(start, normal);

	if (vertexPos + 6 > maxVertices)
		return;

	Vertex* v = vertices + vertexPos;

	v[0] = Vertex(v0.x, v0.y, 0.0f, aa, bb, cc, r, g, b, a);
	v[1] = Vertex(v1.x, v1.y, 0.0f, aa, bb, cc, r, g, b, a);
	v[2] = Vertex(v3.x, v3.y, 0.0f, aa, bb, cc, r, g, b, a);

	v[3] = Vertex(v3.x, v3.y, 0.0f, aa, bb, cc, r, g, b, a);
	v[4] = Vertex(v1.x, v1.y, 0.0f, aa, bb, cc, r, g, b, a);
	v[5] = Vertex(v2.x, v2.y, 0.0f, aa, bb, cc, r, g, b, a);

	drawcommands->bindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLine.get());
	drawcommands->draw(6, 1, vertexPos, 0);

	vertexPos += 6;
}

void VulkanRenderDevice::DrawImageBox(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, GameTexture* gameTexture, float r, float g, float b, float a)
{
	ValidateTexture(gameTexture);

	if (vertexPos + 6 > maxVertices)
		return;

	Vertex* v = vertices + vertexPos;

	v[0] = Vertex(x0, y0, 0.0f, 0.0f, 0.0f, 0.0f, r, g, b, a);
	v[1] = Vertex(x1, y1, 0.0f, 1.0f, 0.0f, 0.0f, r, g, b, a);
	v[2] = Vertex(x3, y3, 0.0f, 0.0f, 1.0f, 0.0f, r, g, b, a);

	v[3] = Vertex(x3, y3, 0.0f, 0.0f, 1.0f, 0.0f, r, g, b, a);
	v[4] = Vertex(x1, y1, 0.0f, 1.0f, 0.0f, 0.0f, r, g, b, a);
	v[5] = Vertex(x2, y2, 0.0f, 1.0f, 1.0f, 0.0f, r, g, b, a);

	drawcommands->bindDescriptorSet(VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayoutTextured.get(), 1, static_cast<VulkanCachedTexture*>(gameTexture->CacheEntry.get())->textureSet.get());
	drawcommands->bindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineTextured.get());
	drawcommands->draw(6, 1, vertexPos, 0);

	vertexPos += 6;
}

void VulkanRenderDevice::End()
{
	// End rendering
	drawcommands->endRenderPass();
	drawcommands->end();

	// Submit command buffer to the graphics queue
	QueueSubmit()
		.AddCommandBuffer(drawcommands.get())
		.AddWait(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, imageAvailableSemaphore.get())
		.AddSignal(renderFinishedSemaphore.get())
		.Execute(device.get(), device->GraphicsQueue, presentFinishedFence.get());

	// Present the frame buffer image
	swapchain->QueuePresent(imageIndex, renderFinishedSemaphore.get());

	// Wait for the swapchain present to finish
	vkWaitForFences(device->device, 1, &presentFinishedFence->fence, VK_TRUE, std::numeric_limits<uint64_t>::max());
	vkResetFences(device->device, 1, &presentFinishedFence->fence);

	transfercommands.reset();
	drawcommands.reset();

	TransferDeleteList = std::make_unique<DeleteList>();
	DrawDeleteList = std::make_unique<DeleteList>();
}

void VulkanRenderDevice::ValidateTexture(GameTexture* gameTexture)
{
	if (gameTexture->CacheEntry)
		return;

	auto cacheEntry = std::make_unique<VulkanCachedTexture>();

	// Create image and view objects

	cacheEntry->textureImage = ImageBuilder()
		.Usage(VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT)
		.Format(VK_FORMAT_R8G8B8A8_UNORM)
		.Size(gameTexture->width, gameTexture->height)
		.DebugName("textureImage")
		.Create(device.get());

	cacheEntry->textureView = ImageViewBuilder()
		.Type(VK_IMAGE_VIEW_TYPE_2D)
		.Image(cacheEntry->textureImage.get(), VK_FORMAT_R8G8B8A8_UNORM)
		.DebugName("textureView")
		.Create(device.get());

	// Create descriptor set for the texture

	if (textureSetsLeft == 0)
	{
		textureSetsLeft = 200;
		textureSetPools.push_back(DescriptorPoolBuilder()
			.AddPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, textureSetsLeft)
			.MaxSets(textureSetsLeft)
			.DebugName("textureSetPool")
			.Create(device.get()));
	}

	cacheEntry->textureSet = textureSetPools.back()->allocate(textureSetLayout.get());
	textureSetsLeft--;

	WriteDescriptors()
		.AddCombinedImageSampler(cacheEntry->textureSet.get(), 0, cacheEntry->textureView.get(), sampler.get(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		.Execute(device.get());

	// Put a texture in the upload buffer

	uint32_t* pixels = (uint32_t*)uploads;
	size_t uploadSize = gameTexture->pixels.size() * sizeof(uint32_t);
	if (uploadSize > uploadBufferSize)
		throw std::runtime_error("Texture upload buffer too small!");

	memcpy(pixels, gameTexture->pixels.data(), uploadSize);

	// Copy the pixels from the upload buffer to the image object and transition the image to the layout suitable for texture sampling

	transfercommands = commandPool->createBuffer();

	transfercommands->begin();

	PipelineBarrier()
		.AddImage(cacheEntry->textureImage.get(), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 0, VK_ACCESS_TRANSFER_WRITE_BIT)
		.Execute(transfercommands.get(), VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);

	VkBufferImageCopy region = {};
	region.imageExtent.width = gameTexture->width;
	region.imageExtent.height = gameTexture->height;
	region.imageExtent.depth = 1;
	region.imageSubresource.layerCount = 1;
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	transfercommands->copyBufferToImage(uploadBuffer->buffer, cacheEntry->textureImage->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	PipelineBarrier()
		.AddImage(cacheEntry->textureImage.get(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT)
		.Execute(transfercommands.get(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);

	transfercommands->end();

	// Submit the command buffer and wait for the device to finish the upload

	QueueSubmit()
		.AddCommandBuffer(transfercommands.get())
		.Execute(device.get(), device->GraphicsQueue);

	vkDeviceWaitIdle(device->device);
	transfercommands.reset();

	cacheEntry->renderdev = this;
	cacheEntry->it = cachedTextures.insert(cachedTextures.end(), cacheEntry.get());

	gameTexture->CacheEntry = std::move(cacheEntry);
}

void VulkanRenderDevice::DestroyCachedTexture(VulkanCachedTexture* cachedTexture)
{
	DrawDeleteList->Add(std::move(cachedTexture->textureSet));
	DrawDeleteList->Add(std::move(cachedTexture->textureView));
	DrawDeleteList->Add(std::move(cachedTexture->textureImage));
	cachedTexture->renderdev = nullptr;
	cachedTextures.erase(cachedTexture->it);
}
