////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkan_shader.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkan_shader.h"
#include "vulkan_utils.h"
#include "vulkan_data.h"
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
        "layout (std140, binding = 0) uniform sceneBuffer {\n"
        "    mat4 viewProj;\n"
        "} scene;\n"
        //"layout (std140, binding = 1) uniform objectBuffer {\n"
        //"    mat4 world;\n"
        //"} object;\n"
        "layout (location = 0) in vec3 pos;\n"
        "layout (location = 1) in vec2 uvs;\n"
        "layout (location = 2) in vec3 normal;\n"
        "layout (location = 0) out vec3 outColor;\n"
        "layout (location = 1) out vec2 outUvs;\n"
        "out gl_PerVertex { \n"
        "    vec4 gl_Position;\n"
        "};\n"
        "void main() {\n"
        "   outColor = normal;\n"
        "   outUvs = uvs;\n"
        "   gl_Position = scene.viewProj * vec4(pos, 1.0);\n"
        "}\n";

    m_fragmentText =
        "#version 400\n"
        "#extension GL_ARB_separate_shader_objects : enable\n"
        "#extension GL_ARB_shading_language_420pack : enable\n"
        "layout (location = 0) in vec3 color;\n"
        "layout (location = 1) in vec2 uvs;\n"
        "layout (location = 0) out vec4 outColor;\n"
        "void main() {\n"
        "   outColor = vec4(color.r, color.g, color.b, uvs.x);\n"
        "}\n";
}

VkShader::~VkShader()
{
    Release();
}

void VkShader::Release()
{
    ReleaseShaderModules();

    if (m_descLayout != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorSetLayout(m_info.device, m_descLayout, NULL);
    }

    if (m_pipelineLayout != VK_NULL_HANDLE)
    {
        vkDestroyPipelineLayout(m_info.device, m_pipelineLayout, NULL);
    }

    if (m_pipeline != VK_NULL_HANDLE)
    {
        vkDestroyPipeline(m_info.device, m_pipeline, NULL);
    }

    if (m_scene.buffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(m_info.device, m_scene.buffer, NULL);
    }

    if (m_scene.memory != VK_NULL_HANDLE)
    {
        vkFreeMemory(m_info.device, m_scene.memory, NULL);
    }

    if (m_descPool != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorPool(m_info.device, m_descPool, NULL);
    }

    Reset();
}

void VkShader::ReleaseShaderModules()
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

    m_sceneData = {};
    m_scene = {};
    m_pipeline = VK_NULL_HANDLE;
    m_pipelineLayout = VK_NULL_HANDLE;
    m_descLayout = VK_NULL_HANDLE;
    m_descSet = VK_NULL_HANDLE;
    m_descPool = VK_NULL_HANDLE;
}

void VkShader::SetActive()
{
    VkWriteDescriptorSet writeDescriptorSet = {};
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.dstSet = m_descSet;
    writeDescriptorSet.descriptorCount = 1;
    writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writeDescriptorSet.pBufferInfo = &m_scene.bufferInfo;
    writeDescriptorSet.dstBinding = 0;
    vkUpdateDescriptorSets(m_info.device, 1, &writeDescriptorSet, 0, nullptr);
}

void VkShader::Bind()
{
    auto& cmd = m_info.SelectedCmd();
    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1, &m_descSet, 0, NULL);
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
    if (!InitDescriptorPool(m_info))
    {
        return "Failed to initialise Descriptor Pool";
    }

    if (!InitDescriptorAndPipelineLayouts(m_info))
    {
        return "Failed to initialise Layouts";
    }

    if (!InitDescriptorSet(m_info))
    {
        return "Failed to initialise Descriptor Set";
    }

    if (!InitUniformBuffer(m_info))
    {
        return "Failed to initialise Uniform Buffer";
    }

    std::string buffer = CompileShader();
    if(!buffer.empty())
    {
        return buffer;
    }

    if (!InitPipeline(m_info))
    {
        return "Failed to initialise Pipeline";
    }

    return "";
}

std::string VkShader::CompileShader()
{
    ReleaseShaderModules();

    VkShaderModuleCreateInfo moduleCreateInfo;
    std::vector<unsigned int> vtx_spv;
    m_stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    m_stages[0].pNext = NULL;
    m_stages[0].pSpecializationInfo = NULL;
    m_stages[0].flags = 0;
    m_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    m_stages[0].pName = "main";

    const std::string vertErrors = Parse(VK_SHADER_STAGE_VERTEX_BIT, m_vertexText.c_str(), vtx_spv);
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

    const std::string fragErrors = Parse(VK_SHADER_STAGE_FRAGMENT_BIT, m_fragmentText.c_str(), frag_spv);
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

void VkShader::SendUniformBuffer()
{
    // TODO: Make this generic for scene
    m_sceneData.viewProj = m_info.viewProjection;

    uint8_t* pData = nullptr;
    CHECK_FAIL(vkMapMemory(m_info.device, m_scene.memory, 0, sizeof(m_sceneData), 0, (void **)&pData));
    memcpy(pData, &m_sceneData, sizeof(m_sceneData));
    vkUnmapMemory(m_info.device, m_scene.memory);
}

std::string VkShader::GetText() const
{
    return m_vertexText + "\n" + m_fragmentText;
}

std::string VkShader::GetAssembly()
{
    return m_vertexAsm + "\n" + m_fragmentAsm;
}

std::string VkShader::Parse(const VkShaderStageFlagBits shader_type,
                            const char* pshader, 
                            std::vector<unsigned int>& spirv)
{
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
        return std::string(shader.getInfoLog()) + " " + shader.getInfoDebugLog();
    }

    program.addShader(&shader);
    if (!program.link(messages))
    {
        return std::string(shader.getInfoLog()) + " " + shader.getInfoDebugLog();
    }

    if (!program.buildReflection())
    {
        return std::string(shader.getInfoLog()) + " " + shader.getInfoDebugLog();
    }

    glslang::GlslangToSpv(*program.getIntermediate(stage), spirv);
    return "";
}

bool VkShader::InitDescriptorAndPipelineLayouts(VulkanData& info)
{
    bool use_texture = false;

    VkDescriptorSetLayoutBinding layoutBindings[2];
    layoutBindings[0].binding = 0;
    layoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    layoutBindings[0].descriptorCount = 1;
    layoutBindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    layoutBindings[0].pImmutableSamplers = NULL;

    if (use_texture)
    {
        layoutBindings[1].binding = 1;
        layoutBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        layoutBindings[1].descriptorCount = 1;
        layoutBindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        layoutBindings[1].pImmutableSamplers = NULL;
    }

    // Next take layout bindings and use them to create a descriptor set layout
    VkDescriptorSetLayoutCreateInfo descriptorLayout = {};
    descriptorLayout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorLayout.pNext = NULL;
    descriptorLayout.bindingCount = use_texture ? 2 : 1;
    descriptorLayout.pBindings = layoutBindings;

    if (CHECK_FAIL(vkCreateDescriptorSetLayout(info.device, &descriptorLayout, NULL, &m_descLayout)))
    {
        return false;
    }

    VulkanUtils::SetDebugName(info,
        (uint64_t)m_descLayout,
        typeid(m_descLayout),
        "DescLayout_" + GetName());

    // Now use the descriptor layout to create a pipeline layout
    VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {};
    pPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pPipelineLayoutCreateInfo.pNext = NULL;
    pPipelineLayoutCreateInfo.pushConstantRangeCount = 0;
    pPipelineLayoutCreateInfo.pPushConstantRanges = NULL;
    pPipelineLayoutCreateInfo.setLayoutCount = 1;
    pPipelineLayoutCreateInfo.pSetLayouts = &m_descLayout;

    if (CHECK_FAIL(vkCreatePipelineLayout(info.device, &pPipelineLayoutCreateInfo, NULL, &m_pipelineLayout)))
    {
        return false;
    }

    VulkanUtils::SetDebugName(info,
        (uint64_t)m_pipelineLayout,
        typeid(m_pipelineLayout),
        "PipelineLayout_" + GetName());

    return true;
}

bool VkShader::InitDescriptorSet(VulkanData& info)
{
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = m_descPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &m_descLayout;
    return !CHECK_FAIL(vkAllocateDescriptorSets(info.device, &allocInfo, &m_descSet));
}

bool VkShader::InitDescriptorPool(VulkanData& info)
{
    bool use_texture = false;

    VkDescriptorPoolSize typeCount[2];
    typeCount[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    typeCount[0].descriptorCount = 1;
    if (use_texture)
    {
        typeCount[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        typeCount[1].descriptorCount = 1;
    }

    VkDescriptorPoolCreateInfo descriptorPool = {};
    descriptorPool.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPool.pNext = NULL;
    descriptorPool.maxSets = 1;
    descriptorPool.poolSizeCount = use_texture ? 2 : 1;
    descriptorPool.pPoolSizes = typeCount;

    return !CHECK_FAIL(vkCreateDescriptorPool(info.device, &descriptorPool, NULL, &m_descPool));
}

bool VkShader::InitPipeline(VulkanData& info)
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
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

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

    //------------------------------------------------
    // TODO: Make this generic for scene
    const int positionComponents = 3;
    const int uvComponents = 2;
    const int normalComponents = 3;
    const int attributeCount = 3;

    VkVertexInputBindingDescription vertexInputBinding = {};
    vertexInputBinding.binding = 0;
    vertexInputBinding.stride = (positionComponents + normalComponents + uvComponents) * sizeof(float);
    vertexInputBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    std::array<VkVertexInputAttributeDescription, attributeCount> vertexInputAttributs;
    vertexInputAttributs[0].binding = 0;
    vertexInputAttributs[0].location = 0;
    vertexInputAttributs[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    vertexInputAttributs[0].offset = 0;
    vertexInputAttributs[1].binding = 0;
    vertexInputAttributs[1].location = 1;
    vertexInputAttributs[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    vertexInputAttributs[1].offset = positionComponents * sizeof(float);
    vertexInputAttributs[2].binding = 0;
    vertexInputAttributs[2].location = 2;
    vertexInputAttributs[2].format = VK_FORMAT_R32G32B32_SFLOAT;
    vertexInputAttributs[2].offset = (positionComponents + uvComponents) * sizeof(float);

    VkPipelineVertexInputStateCreateInfo vertexInputState = {};
    vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputState.vertexBindingDescriptionCount = 1;
    vertexInputState.pVertexBindingDescriptions = &vertexInputBinding;
    vertexInputState.vertexAttributeDescriptionCount = attributeCount;
    vertexInputState.pVertexAttributeDescriptions = vertexInputAttributs.data();
    //------------------------------------------------

    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.layout = m_pipelineLayout;
    pipelineCreateInfo.renderPass = info.renderPass;
    pipelineCreateInfo.stageCount = static_cast<uint32_t>(m_stages.size());
    pipelineCreateInfo.pStages = m_stages.data();
    pipelineCreateInfo.pVertexInputState = &vertexInputState;
    pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
    pipelineCreateInfo.pRasterizationState = &rasterizationState;
    pipelineCreateInfo.pColorBlendState = &colorBlendState;
    pipelineCreateInfo.pMultisampleState = &multisampleState;
    pipelineCreateInfo.pViewportState = &viewportState;
    pipelineCreateInfo.pDepthStencilState = &depthStencilState;
    pipelineCreateInfo.renderPass = info.renderPass;
    pipelineCreateInfo.pDynamicState = &dynamicState;

    if (CHECK_FAIL(vkCreateGraphicsPipelines(info.device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &m_pipeline)))
    {
        return false;
    }

    VulkanUtils::SetDebugName(info,
        (uint64_t)m_pipeline, typeid(m_pipeline),
        "Pipeline" + GetName());

    return true;
}

bool VkShader::InitUniformBuffer(VulkanData& info)
{
    VkMemoryRequirements memReqs;
    VkBufferCreateInfo bufferInfo = {};
    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;
    allocInfo.allocationSize = 0;
    allocInfo.memoryTypeIndex = 0;

    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeof(m_sceneData);
    bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

    if (CHECK_FAIL(vkCreateBuffer(info.device, &bufferInfo, nullptr, &m_scene.buffer)))
    {
        return false;
    }

    vkGetBufferMemoryRequirements(info.device, m_scene.buffer, &memReqs);
    allocInfo.allocationSize = memReqs.size;
    if (CHECK_FAIL(VulkanUtils::MemoryTypeFromProperties(info, memReqs.memoryTypeBits,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &allocInfo.memoryTypeIndex)))
    {
        return false;
    }

    if (CHECK_FAIL(vkAllocateMemory(info.device, &allocInfo, nullptr, &m_scene.memory)))
    {
        return false;
    }

    if (CHECK_FAIL(vkBindBufferMemory(info.device, m_scene.buffer, m_scene.memory, 0)))
    {
        return false;
    }

    m_scene.bufferInfo.buffer = m_scene.buffer;
    m_scene.bufferInfo.offset = 0;
    m_scene.bufferInfo.range = sizeof(m_sceneData);

    return true;
}