#pragma once

#include "Game/RenderDevice.h"
#include <zvulkan/vulkanobjects.h>

struct Vertex
{
	Vertex() = default;
	Vertex(float x, float y, float z, float u, float v, float r, float g, float b, float a) : x(x), y(y), z(z), u(u), v(v), r(r), g(g), b(b), a(a) { }

	float x, y, z;
	float u, v;
	float r, g, b, a;
};

struct Uniforms
{
	float ProjectionMatrix[16];
	float ViewMatrix[16];
};

class VulkanRenderDevice : public RenderDevice
{
public:
	VulkanRenderDevice(Widget* viewport);
	~VulkanRenderDevice();

	bool Begin() override;
	void DrawImage(int x, int y, int width, int height, GameTexture* gameTexture) override;
	void End() override;

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

	std::unique_ptr<VulkanDescriptorSetLayout> uniformSetLayout;
	std::unique_ptr<VulkanDescriptorSetLayout> textureSetLayout;

	std::unique_ptr<VulkanPipelineLayout> pipelineLayoutNoTex;
	std::unique_ptr<VulkanPipelineLayout> pipelineLayoutTextured;

	std::unique_ptr<VulkanRenderPass> renderPass;

	std::unique_ptr<VulkanPipeline> pipelineNoTex;
	std::unique_ptr<VulkanPipeline> pipelineTextured;

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
	std::unique_ptr<VulkanImage> textureImage;
	std::unique_ptr<VulkanImageView> textureView;

	std::unique_ptr<VulkanDescriptorPool> uniformSetPool;
	std::unique_ptr<VulkanDescriptorSet> uniformSet;

	std::unique_ptr<VulkanDescriptorPool> textureSetPool;
	std::unique_ptr<VulkanDescriptorSet> textureSet;

	std::unique_ptr<VulkanCommandBuffer> transfercommands;
	std::unique_ptr<VulkanCommandBuffer> drawcommands;
};
