////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkanshader.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkanshader.h"
#include "vulkanutils.h"
#include "vulkandata.h"
#include "shader.h"
#include "glslang/SPIRV/GlslangToSpv.h"

namespace
{
    const std::string VS("Vertex Shader: ");   ///< Text for vertex shader diagnostics
    const std::string FS("Fragment Shader: "); ///< Text for fragment shader diagnostics

    EShLanguage FindLanguage(const VkShaderStageFlagBits shader_type)
    {
        switch (shader_type)
        {
        case VK_SHADER_STAGE_VERTEX_BIT:
            return EShLangVertex;
        case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
            return EShLangTessControl;
        case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
            return EShLangTessEvaluation;
        case VK_SHADER_STAGE_GEOMETRY_BIT:
            return EShLangGeometry;
        case VK_SHADER_STAGE_FRAGMENT_BIT:
            return EShLangFragment;
        case VK_SHADER_STAGE_COMPUTE_BIT:
            return EShLangCompute;
        default:
            return EShLangVertex;
        }
    }

    void InitResources(TBuiltInResource& resources)
    {
        resources.maxLights = 32;
        resources.maxClipPlanes = 6;
        resources.maxTextureUnits = 32;
        resources.maxTextureCoords = 32;
        resources.maxVertexAttribs = 64;
        resources.maxVertexUniformComponents = 4096;
        resources.maxVaryingFloats = 64;
        resources.maxVertexTextureImageUnits = 32;
        resources.maxCombinedTextureImageUnits = 80;
        resources.maxTextureImageUnits = 32;
        resources.maxFragmentUniformComponents = 4096;
        resources.maxDrawBuffers = 32;
        resources.maxVertexUniformVectors = 128;
        resources.maxVaryingVectors = 8;
        resources.maxFragmentUniformVectors = 16;
        resources.maxVertexOutputVectors = 16;
        resources.maxFragmentInputVectors = 15;
        resources.minProgramTexelOffset = -8;
        resources.maxProgramTexelOffset = 7;
        resources.maxClipDistances = 8;
        resources.maxComputeWorkGroupCountX = 65535;
        resources.maxComputeWorkGroupCountY = 65535;
        resources.maxComputeWorkGroupCountZ = 65535;
        resources.maxComputeWorkGroupSizeX = 1024;
        resources.maxComputeWorkGroupSizeY = 1024;
        resources.maxComputeWorkGroupSizeZ = 64;
        resources.maxComputeUniformComponents = 1024;
        resources.maxComputeTextureImageUnits = 16;
        resources.maxComputeImageUniforms = 8;
        resources.maxComputeAtomicCounters = 8;
        resources.maxComputeAtomicCounterBuffers = 1;
        resources.maxVaryingComponents = 60;
        resources.maxVertexOutputComponents = 64;
        resources.maxGeometryInputComponents = 64;
        resources.maxGeometryOutputComponents = 128;
        resources.maxFragmentInputComponents = 128;
        resources.maxImageUnits = 8;
        resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
        resources.maxCombinedShaderOutputResources = 8;
        resources.maxImageSamples = 0;
        resources.maxVertexImageUniforms = 0;
        resources.maxTessControlImageUniforms = 0;
        resources.maxTessEvaluationImageUniforms = 0;
        resources.maxGeometryImageUniforms = 0;
        resources.maxFragmentImageUniforms = 8;
        resources.maxCombinedImageUniforms = 8;
        resources.maxGeometryTextureImageUnits = 16;
        resources.maxGeometryOutputVertices = 256;
        resources.maxGeometryTotalOutputComponents = 1024;
        resources.maxGeometryUniformComponents = 1024;
        resources.maxGeometryVaryingComponents = 64;
        resources.maxTessControlInputComponents = 128;
        resources.maxTessControlOutputComponents = 128;
        resources.maxTessControlTextureImageUnits = 16;
        resources.maxTessControlUniformComponents = 1024;
        resources.maxTessControlTotalOutputComponents = 4096;
        resources.maxTessEvaluationInputComponents = 128;
        resources.maxTessEvaluationOutputComponents = 128;
        resources.maxTessEvaluationTextureImageUnits = 16;
        resources.maxTessEvaluationUniformComponents = 1024;
        resources.maxTessPatchComponents = 120;
        resources.maxPatchVertices = 32;
        resources.maxTessGenLevel = 64;
        resources.maxViewports = 16;
        resources.maxVertexAtomicCounters = 0;
        resources.maxTessControlAtomicCounters = 0;
        resources.maxTessEvaluationAtomicCounters = 0;
        resources.maxGeometryAtomicCounters = 0;
        resources.maxFragmentAtomicCounters = 8;
        resources.maxCombinedAtomicCounters = 8;
        resources.maxAtomicCounterBindings = 1;
        resources.maxVertexAtomicCounterBuffers = 0;
        resources.maxTessControlAtomicCounterBuffers = 0;
        resources.maxTessEvaluationAtomicCounterBuffers = 0;
        resources.maxGeometryAtomicCounterBuffers = 0;
        resources.maxFragmentAtomicCounterBuffers = 1;
        resources.maxCombinedAtomicCounterBuffers = 1;
        resources.maxAtomicCounterBufferSize = 16384;
        resources.maxTransformFeedbackBuffers = 4;
        resources.maxTransformFeedbackInterleavedComponents = 64;
        resources.maxCullDistances = 8;
        resources.maxCombinedClipAndCullDistances = 8;
        resources.maxSamples = 4;
        resources.limits.nonInductiveForLoops = 1;
        resources.limits.whileLoops = 1;
        resources.limits.doWhileLoops = 1;
        resources.limits.generalUniformIndexing = 1;
        resources.limits.generalAttributeMatrixVectorIndexing = 1;
        resources.limits.generalVaryingIndexing = 1;
        resources.limits.generalSamplerIndexing = 1;
        resources.limits.generalVariableIndexing = 1;
        resources.limits.generalConstantMatrixVectorIndexing = 1;
    }

    std::string GlslToSpb(const VkShaderStageFlagBits shader_type, const char* pshader, std::vector<unsigned int>& spirv)
    {
        std::string errorBuffer;

        EShLanguage stage = FindLanguage(shader_type);
        glslang::TShader shader(stage);
        glslang::TProgram program;
        const char *shaderStrings[1];

        TBuiltInResource resources;
        InitResources(resources);

        // Enable SPIR-V and Vulkan rules when parsing GLSL
        EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);

        shaderStrings[0] = pshader;
        shader.setStrings(shaderStrings, 1);

        if (!shader.parse(&resources, 100, false, messages))
        {
            errorBuffer += shader.getInfoLog();
            errorBuffer += shader.getInfoDebugLog();
            return errorBuffer;
        }

        program.addShader(&shader);

        // Program-level processing
        if (!program.link(messages))
        {
            errorBuffer += shader.getInfoLog();
            errorBuffer += shader.getInfoDebugLog();
            return errorBuffer;
        }

        glslang::GlslangToSpv(*program.getIntermediate(stage), spirv);
        return errorBuffer;
    }
}

VkShader::VkShader(VulkanData& info, const Shader& shader)
    : m_info(info)
    , m_shader(shader)
{
    m_stages.resize(2);
    Reset();

    // TODO: Make this generic for scene
    m_vertexText =
        "#version 400\n"
        "#extension GL_ARB_separate_shader_objects : enable\n"
        "#extension GL_ARB_shading_language_420pack : enable\n"
        "layout (std140, binding = 0) uniform bufferVals {\n"
        "    mat4 mvp;\n"
        "} myBufferVals;\n"
        "layout (location = 0) in vec4 pos;\n"
        "layout (location = 1) in vec4 inColor;\n"
        "layout (location = 0) out vec4 outColor;\n"
        "out gl_PerVertex { \n"
        "    vec4 gl_Position;\n"
        "};\n"
        "void main() {\n"
        "   outColor = inColor;\n"
        "   gl_Position = myBufferVals.mvp * pos;\n"
        "}\n";

    m_fragmentText =
        "#version 400\n"
        "#extension GL_ARB_separate_shader_objects : enable\n"
        "#extension GL_ARB_shading_language_420pack : enable\n"
        "layout (location = 0) in vec4 color;\n"
        "layout (location = 0) out vec4 outColor;\n"
        "void main() {\n"
        "   outColor = color;\n"
        "}\n";
}

VkShader::~VkShader()
{
    Release();
}

void VkShader::Release()
{
    ReleaseShader();

    if (m_pipeline != VK_NULL_HANDLE)
    {
        vkDestroyPipeline(m_info.device, m_pipeline, NULL);
    }

    if (m_buffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(m_info.device, m_buffer, NULL);
    }

    if (m_memory != VK_NULL_HANDLE)
    {
        vkFreeMemory(m_info.device, m_memory, NULL);
    }

    Reset();
}

void VkShader::ReleaseShader()
{
    for (int i = 0; i < (int)m_stages.size(); i++)
    {
        if (m_stages[i].module != VK_NULL_HANDLE)
        {
            vkDestroyShaderModule(m_info.device, m_stages[i].module, NULL);
            m_stages[i].module = VK_NULL_HANDLE;
        }
    }
}

void VkShader::Reset()
{
    for (int i = 0; i < (int)m_stages.size(); i++)
    {
        m_stages[i].module = VK_NULL_HANDLE;
    }

    m_data = {};
    m_buffer = VK_NULL_HANDLE;
    m_memory = VK_NULL_HANDLE;
    m_bufferInfo = {};
    m_pipeline = VK_NULL_HANDLE;
}

void VkShader::SetActive()
{
    VkWriteDescriptorSet writeDescriptorSet = {};
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.dstSet = m_info.descSet;
    writeDescriptorSet.descriptorCount = 1;
    writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writeDescriptorSet.pBufferInfo = &m_bufferInfo;
    writeDescriptorSet.dstBinding = 0;
    vkUpdateDescriptorSets(m_info.device, 1, &writeDescriptorSet, 0, nullptr);
}

void VkShader::Bind(VkCommandBuffer cmd)
{
    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_info.pipelineLayout, 0, 1, &m_info.descSet, 0, NULL);
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
}

const std::string& VkShader::GetName() const
{
    return m_shader.Name();
}

bool VkShader::InitFramework()
{
    return glslang::InitializeProcess();
}

void VkShader::ReleaseFramework()
{
    glslang::FinalizeProcess();
}

std::string VkShader::Initialise()
{
    if (!InitUniformBuffer())
    {
        return "Failed to initialise Uniform Buffer";
    }

    std::string buffer = CompileShader();
    if(!buffer.empty())
    {
        return buffer;
    }

    if (!InitPipeline())
    {
        return "Failed to initialise Pipeline";
    }

    return "";
}

std::string VkShader::CompileShader()
{
    ReleaseShader();

    VkShaderModuleCreateInfo moduleCreateInfo;
    std::vector<unsigned int> vtx_spv;
    m_stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    m_stages[0].pNext = NULL;
    m_stages[0].pSpecializationInfo = NULL;
    m_stages[0].flags = 0;
    m_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    m_stages[0].pName = "main";

    const std::string vertErrors = GlslToSpb(VK_SHADER_STAGE_VERTEX_BIT, m_vertexText.c_str(), vtx_spv);
    if (!vertErrors.empty())
    {
        return VS + vertErrors;
    }

    moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    moduleCreateInfo.pNext = NULL;
    moduleCreateInfo.flags = 0;
    moduleCreateInfo.codeSize = vtx_spv.size() * sizeof(unsigned int);
    moduleCreateInfo.pCode = vtx_spv.data();

    VkResult res = vkCreateShaderModule(m_info.device, &moduleCreateInfo, NULL, &m_stages[0].module);
    if (VulkanUtils::Failed(res))
    {
        return VS + VulkanUtils::GetFailText(res);
    }

    std::vector<unsigned int> frag_spv;
    m_stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    m_stages[1].pNext = NULL;
    m_stages[1].pSpecializationInfo = NULL;
    m_stages[1].flags = 0;
    m_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    m_stages[1].pName = "main";

    const std::string fragErrors = GlslToSpb(VK_SHADER_STAGE_FRAGMENT_BIT, m_fragmentText.c_str(), frag_spv);
    if(!fragErrors.empty())
    {
        return FS + fragErrors;
    }

    moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    moduleCreateInfo.pNext = NULL;
    moduleCreateInfo.flags = 0;
    moduleCreateInfo.codeSize = frag_spv.size() * sizeof(unsigned int);
    moduleCreateInfo.pCode = frag_spv.data();
    if(CHECK_FAIL(vkCreateShaderModule(m_info.device, &moduleCreateInfo, NULL, &m_stages[1].module)))
    if (VulkanUtils::Failed(res))
    {
        return FS + VulkanUtils::GetFailText(res);
    }

    for (int i = 0; i < (int)m_stages.size(); i++)
    {
        VulkanUtils::SetDebugName(m_info,
            (uint64_t)m_stages[0].module, typeid(m_stages[0].module),
            "Shader Module" + std::to_string(i) + GetName());
    }

    return "";
}

bool VkShader::InitUniformBuffer()
{
    VkMemoryRequirements memReqs;
    VkBufferCreateInfo bufferInfo = {};
    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;
    allocInfo.allocationSize = 0;
    allocInfo.memoryTypeIndex = 0;

    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeof(m_data);
    bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

    if (CHECK_FAIL(vkCreateBuffer(m_info.device, &bufferInfo, nullptr, &m_buffer)))
    {
        return false;
    }

    vkGetBufferMemoryRequirements(m_info.device, m_buffer, &memReqs);
    allocInfo.allocationSize = memReqs.size;
    if(CHECK_FAIL(VulkanUtils::MemoryTypeFromProperties(m_info, memReqs.memoryTypeBits,
                                                        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                                        &allocInfo.memoryTypeIndex)))
    {
        return false;
    }

    if (CHECK_FAIL(vkAllocateMemory(m_info.device, &allocInfo, nullptr, &m_memory)))
    {
        return false;
    }

    if (CHECK_FAIL(vkBindBufferMemory(m_info.device, m_buffer, m_memory, 0)))
    {
        return false;
    }

    m_bufferInfo.buffer = m_buffer;
    m_bufferInfo.offset = 0;
    m_bufferInfo.range = sizeof(m_data);

    // Update uniform data
    uint8_t* pData = nullptr;
    if (CHECK_FAIL(vkMapMemory(m_info.device, m_memory, 0, sizeof(m_data), 0, (void **)&pData)))
    {
        return false;
    }
    memcpy(pData, &m_data, sizeof(m_data));
    vkUnmapMemory(m_info.device, m_memory);

    return true;
}

bool VkShader::InitPipeline()
{
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
    inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkPipelineRasterizationStateCreateInfo rasterizationState = {};
    rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizationState.cullMode = VK_CULL_MODE_NONE;
    rasterizationState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizationState.depthClampEnable = VK_FALSE;
    rasterizationState.rasterizerDiscardEnable = VK_FALSE;
    rasterizationState.depthBiasEnable = VK_FALSE;
    rasterizationState.lineWidth = 1.0f;

    VkPipelineColorBlendAttachmentState blendAttachmentState[1] = {};
    blendAttachmentState[0].colorWriteMask = 0xf;
    blendAttachmentState[0].blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlendState = {};
    colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendState.attachmentCount = 1;
    colorBlendState.pAttachments = blendAttachmentState;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = VulkanUtils::NUM_VIEWPORTS_AND_SCISSORS;
    viewportState.scissorCount = VulkanUtils::NUM_VIEWPORTS_AND_SCISSORS;

    std::vector<VkDynamicState> dynamicStateEnables;
    dynamicStateEnables.push_back(VK_DYNAMIC_STATE_VIEWPORT);
    dynamicStateEnables.push_back(VK_DYNAMIC_STATE_SCISSOR);

    VkPipelineDynamicStateCreateInfo dynamicState = {};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.pDynamicStates = dynamicStateEnables.data();
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());

    VkPipelineDepthStencilStateCreateInfo depthStencilState = {};
    depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilState.depthTestEnable = VK_TRUE;
    depthStencilState.depthWriteEnable = VK_TRUE;
    depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    depthStencilState.depthBoundsTestEnable = VK_FALSE;
    depthStencilState.back.failOp = VK_STENCIL_OP_KEEP;
    depthStencilState.back.passOp = VK_STENCIL_OP_KEEP;
    depthStencilState.back.compareOp = VK_COMPARE_OP_ALWAYS;
    depthStencilState.stencilTestEnable = VK_FALSE;
    depthStencilState.front = depthStencilState.back;

    VkPipelineMultisampleStateCreateInfo multisampleState = {};
    multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleState.rasterizationSamples = VulkanUtils::NUM_SAMPLES;
    multisampleState.pSampleMask = nullptr;

    VkVertexInputBindingDescription vertexInputBinding = {};
    vertexInputBinding.binding = 0;
    vertexInputBinding.stride = sizeof(Vertex);
    vertexInputBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    // TODO: Make this generic for scene
    std::array<VkVertexInputAttributeDescription, 2> vertexInputAttributs;
    vertexInputAttributs[0].binding = 0;
    vertexInputAttributs[0].location = 0;
    vertexInputAttributs[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    vertexInputAttributs[0].offset = offsetof(Vertex, position);
    vertexInputAttributs[1].binding = 0;
    vertexInputAttributs[1].location = 1;
    vertexInputAttributs[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    vertexInputAttributs[1].offset = offsetof(Vertex, color);

    // TODO: Make this generic for scene
    VkPipelineVertexInputStateCreateInfo vertexInputState = {};
    vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputState.vertexBindingDescriptionCount = 1;
    vertexInputState.pVertexBindingDescriptions = &vertexInputBinding;
    vertexInputState.vertexAttributeDescriptionCount = 2;
    vertexInputState.pVertexAttributeDescriptions = vertexInputAttributs.data();

    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.layout = m_info.pipelineLayout;
    pipelineCreateInfo.renderPass = m_info.renderPass;
    pipelineCreateInfo.stageCount = static_cast<uint32_t>(m_stages.size());
    pipelineCreateInfo.pStages = m_stages.data();
    pipelineCreateInfo.pVertexInputState = &vertexInputState;
    pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
    pipelineCreateInfo.pRasterizationState = &rasterizationState;
    pipelineCreateInfo.pColorBlendState = &colorBlendState;
    pipelineCreateInfo.pMultisampleState = &multisampleState;
    pipelineCreateInfo.pViewportState = &viewportState;
    pipelineCreateInfo.pDepthStencilState = &depthStencilState;
    pipelineCreateInfo.renderPass = m_info.renderPass;
    pipelineCreateInfo.pDynamicState = &dynamicState;

    if (CHECK_FAIL(vkCreateGraphicsPipelines(m_info.device, m_info.pipelineCache, 1, &pipelineCreateInfo, nullptr, &m_pipeline)))
    {
        return false;
    }

    VulkanUtils::SetDebugName(m_info, 
        (uint64_t)m_pipeline, typeid(m_pipeline),
        "Pipeline" + GetName());

    return true;
}

std::string VkShader::GetText() const
{
    return m_vertexText + "\n" + m_fragmentText;
}

std::string VkShader::GetAssembly()
{
    return m_vertexAsm + "\n" + m_fragmentAsm;
}