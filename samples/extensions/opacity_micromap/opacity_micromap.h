/* Copyright (c) 2019-2025, Sascha Willems
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 the "License";
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Basic example for ray tracing opacity micromap using VK_EXT_opacity_micromap
 */

#pragma once

#include "api_vulkan_sample.h"

class OpacityMicromap : public ApiVulkanSample
{
  public:
	// Holds data for a scratch buffer used as temporary storage during micromap and acceleration structure builds
	struct ScratchBuffer
	{
		uint64_t       device_address;
		VkBuffer       handle;
		VkDeviceMemory memory;
	};

	// Wraps all data required for an acceleration structure
	struct AccelerationStructure
	{
		VkAccelerationStructureKHR          handle = VK_NULL_HANDLE;
		uint64_t                            device_address{0};
		std::unique_ptr<vkb::core::BufferC> buffer;
	};

	struct Micromap
	{
		VkMicromapEXT                       handle = VK_NULL_HANDLE;
		std::unique_ptr<vkb::core::BufferC> buffer;
		std::unique_ptr<vkb::core::BufferC> data_buffer;
		std::unique_ptr<vkb::core::BufferC> triangle_buffer;
		std::unique_ptr<vkb::core::BufferC> index_buffer;
	};

	VkPhysicalDeviceRayTracingPipelinePropertiesKHR  ray_tracing_pipeline_properties{};
	VkPhysicalDeviceAccelerationStructureFeaturesKHR acceleration_structure_features{};
	VkPhysicalDeviceOpacityMicromapPropertiesEXT     opacity_micromap_properties{};

	AccelerationStructure bottom_level_acceleration_structure;
	AccelerationStructure top_level_acceleration_structure;
	Micromap              opacity_micromap;
	Texture               alpha_mask_texture;

	std::unique_ptr<vkb::core::BufferC>               vertex_buffer;
	std::unique_ptr<vkb::core::BufferC>               index_buffer;
	uint32_t                                          index_count;
	std::vector<VkRayTracingShaderGroupCreateInfoKHR> shader_groups{};

	std::unique_ptr<vkb::core::BufferC> raygen_shader_binding_table;
	std::unique_ptr<vkb::core::BufferC> miss_shader_binding_table;
	std::unique_ptr<vkb::core::BufferC> hit_shader_binding_table;

	struct StorageImage
	{
		VkDeviceMemory memory;
		VkImage        image = VK_NULL_HANDLE;
		VkImageView    view;
		VkFormat       format;
		uint32_t       width;
		uint32_t       height;
	} storage_image;

	struct UniformData
	{
		glm::mat4 view_inverse;
		glm::mat4 proj_inverse;
	} uniform_data;
	std::unique_ptr<vkb::core::BufferC> ubo;

	VkPipeline            pipeline;
	VkPipelineLayout      pipeline_layout;
	VkDescriptorSet       descriptor_set;
	VkDescriptorSetLayout descriptor_set_layout;

	OpacityMicromap();
	~OpacityMicromap();

	void          request_gpu_features(vkb::core::PhysicalDeviceC &gpu) override;
	uint64_t      get_buffer_device_address(VkBuffer buffer);
	ScratchBuffer create_scratch_buffer(VkDeviceSize size);
	void          delete_scratch_buffer(ScratchBuffer &scratch_buffer);
	void          create_storage_image();
	void          create_opacity_micromap();
	void          create_bottom_level_acceleration_structure();
	void          create_top_level_acceleration_structure();
	void          delete_micromap(Micromap &micromap);
	void          delete_acceleration_structure(AccelerationStructure &acceleration_structure);
	void          create_scene();
	void          create_shader_binding_tables();
	void          create_descriptor_sets();
	void          create_ray_tracing_pipeline();
	void          create_uniform_buffer();
	void          build_command_buffers() override;
	void          update_uniform_buffers();
	void          draw();
	bool          prepare(const vkb::ApplicationOptions &options) override;
	virtual void  render(float delta_time) override;
};

std::unique_ptr<vkb::VulkanSampleC> create_opacity_micromap();
