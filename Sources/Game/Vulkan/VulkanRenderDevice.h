#pragma once

#include "Game/RenderDevice.h"
#include <zvulkan/vulkanobjects.h>

class VulkanRenderDevice;

struct Vertex
{
	Vertex() = default;
	Vertex(float x, float y, float z, float s, float t, float p, float r, float g, float b, float a) : x(x), y(y), z(z), s(s), t(t), p(p), r(r), g(g), b(b), a(a) { }

	float x, y, z;
	float s, t, p;
	float r, g, b, a;
};

struct Uniforms
{
	float ProjectionMatrix[16];
	float ViewMatrix[16];
	float ViewportX;
	float ViewportY;
	float ViewportScaleX;
	float ViewportScaleY;
};

class VulkanCachedTexture : public CachedTexture
{
public:
	~VulkanCachedTexture();

	VulkanRenderDevice* renderdev = nullptr;
	std::list<VulkanCachedTexture*>::iterator it;
	std::unique_ptr<VulkanImage> textureImage;
	std::unique_ptr<VulkanImageView> textureView;
	std::unique_ptr<VulkanDescriptorSet> textureSet;
};

class VulkanRenderDevice : public RenderDevice
{
public:
	VulkanRenderDevice(Widget* viewport);
	~VulkanRenderDevice();

	bool Begin() override;
	void DrawLine(int x1, int y1, int x2, int y2, float r, float g, float b, float a) override;
	void DrawImageBox(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, GameTexture* gameTexture, float r, float g, float b, float a) override;
	void End() override;

	void DestroyCachedTexture(VulkanCachedTexture* cachedTexture);

private:
	void ValidateTexture(GameTexture* gameTexture);

	Widget* viewport = nullptr;

	std::shared_ptr<VulkanInstance> instance;
	std::shared_ptr<VulkanSurface> surface;
	std::shared_ptr<VulkanDevice> device;
	std::shared_ptr<VulkanSwapChain> swapchain;

	std::unique_ptr<VulkanCommandPool> commandPool;

	std::unique_ptr<VulkanSemaphore> imageAvailableSemaphore;
	std::unique_ptr<VulkanSemaphore> renderFinishedSemaphore;
	std::unique_ptr<VulkanFence> presentFinishedFence;

	std::vector<std::unique_ptr<VulkanFramebuffer>> framebuffers;
	int width = 0;
	int height = 0;
	int lastWidth = 0;
	int lastHeight = 0;
	int imageIndex = -1;

	std::unique_ptr<VulkanShader> vertexShader;
	std::unique_ptr<VulkanShader> fragmentShaderNoTex;
	std::unique_ptr<VulkanShader> fragmentShaderTextured;
	std::unique_ptr<VulkanShader> fragmentShaderLine;

	std::unique_ptr<VulkanDescriptorSetLayout> uniformSetLayout;
	std::unique_ptr<VulkanDescriptorSetLayout> textureSetLayout;

	std::unique_ptr<VulkanPipelineLayout> pipelineLayoutNoTex;
	std::unique_ptr<VulkanPipelineLayout> pipelineLayoutTextured;

	std::unique_ptr<VulkanRenderPass> renderPass;

	std::unique_ptr<VulkanPipeline> pipelineNoTex;
	std::unique_ptr<VulkanPipeline> pipelineTextured;
	std::unique_ptr<VulkanPipeline> pipelineLine;

	size_t maxVertices = 1'000'000;
	std::unique_ptr<VulkanBuffer> vertexBuffer;
	Vertex* vertices = nullptr;
	size_t vertexPos = 0;

	std::unique_ptr<VulkanBuffer> uniformsBuffer;
	Uniforms* uniforms = nullptr;

	size_t uploadBufferSize = 16 * 1024 * 1024;
	std::unique_ptr<VulkanBuffer> uploadBuffer;
	uint8_t* uploads = nullptr;

	std::unique_ptr<VulkanSampler> sampler;

	std::unique_ptr<VulkanDescriptorPool> uniformSetPool;
	std::unique_ptr<VulkanDescriptorSet> uniformSet;

	std::vector<std::unique_ptr<VulkanDescriptorPool>> textureSetPools;
	int textureSetsLeft = 0;
	std::list<VulkanCachedTexture*> cachedTextures;

	std::unique_ptr<VulkanCommandBuffer> transfercommands;
	std::unique_ptr<VulkanCommandBuffer> drawcommands;

	class DeleteList
	{
	public:
		std::vector<std::unique_ptr<VulkanBuffer>> Buffers;
		std::vector<std::unique_ptr<VulkanSampler>> Samplers;
		std::vector<std::unique_ptr<VulkanImage>> Images;
		std::vector<std::unique_ptr<VulkanImageView>> ImageViews;
		std::vector<std::unique_ptr<VulkanFramebuffer>> Framebuffers;
		std::vector<std::unique_ptr<VulkanAccelerationStructure>> AccelStructs;
		std::vector<std::unique_ptr<VulkanDescriptorPool>> DescriptorPools;
		std::vector<std::unique_ptr<VulkanDescriptorSet>> Descriptors;
		std::vector<std::unique_ptr<VulkanShader>> Shaders;
		std::vector<std::unique_ptr<VulkanCommandBuffer>> CommandBuffers;
		size_t TotalSize = 0;

		void Add(std::unique_ptr<VulkanBuffer> obj) { if (obj) { TotalSize += obj->size; Buffers.push_back(std::move(obj)); } }
		void Add(std::unique_ptr<VulkanSampler> obj) { if (obj) { Samplers.push_back(std::move(obj)); } }
		void Add(std::unique_ptr<VulkanImage> obj) { if (obj) { Images.push_back(std::move(obj)); } }
		void Add(std::unique_ptr<VulkanImageView> obj) { if (obj) { ImageViews.push_back(std::move(obj)); } }
		void Add(std::unique_ptr<VulkanFramebuffer> obj) { if (obj) { Framebuffers.push_back(std::move(obj)); } }
		void Add(std::unique_ptr<VulkanAccelerationStructure> obj) { if (obj) { AccelStructs.push_back(std::move(obj)); } }
		void Add(std::unique_ptr<VulkanDescriptorPool> obj) { if (obj) { DescriptorPools.push_back(std::move(obj)); } }
		void Add(std::unique_ptr<VulkanDescriptorSet> obj) { if (obj) { Descriptors.push_back(std::move(obj)); } }
		void Add(std::unique_ptr<VulkanCommandBuffer> obj) { if (obj) { CommandBuffers.push_back(std::move(obj)); } }
		void Add(std::unique_ptr<VulkanShader> obj) { if (obj) { Shaders.push_back(std::move(obj)); } }
	};

	std::unique_ptr<DeleteList> TransferDeleteList = std::make_unique<DeleteList>();
	std::unique_ptr<DeleteList> DrawDeleteList = std::make_unique<DeleteList>();
};

inline VulkanCachedTexture::~VulkanCachedTexture()
{
	if (renderdev)
		renderdev->DestroyCachedTexture(this);
}
