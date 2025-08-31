#include "renderer.h"

#include "client.h"

#include <glm/gtc/type_ptr.hpp>

#include "components/model_filter.h"
#include "components/transform.h"
#include "glm/gtc/quaternion.hpp"
#include "glad/glad.h"

#include "glm/mat4x4.hpp"

namespace OGLGAME
{
    Renderer::Renderer()
    {
        glDepthFunc(GL_LESS);
        glEnable(GL_DEPTH_TEST);
    }

    void Renderer::Render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const ResourceSystem& resourceSystem = Client::S_GetInstance().GetResourceSystem();
        const Scene& scene = Client::S_GetInstance().GetScene();
        Scene::GameObjectHolder* pGameObjects = scene.GetGameObjects();
        const size_t gameObjectCount = scene.GetGameObjectCount();

        GameObject* pPlayerObj = Client::S_GetInstance().GetPlayerObject();
        const auto* pPlayerTransform = pPlayerObj->GetComponent<Components::Transform>();

        glm::mat4 view = glm::translate(glm::mat4(1.0f), pPlayerTransform->m_position) * glm::mat4_cast(pPlayerTransform->m_rotation);
        view = glm::inverse(view);
        const glm::mat4 perspective = glm::perspective(90.0f, 1.0f, 0.03f, 3000.0f);
        const glm::mat4 VP = perspective * view;
        size_t renderedEntityCount = 0;
        for (size_t i = 0; renderedEntityCount != gameObjectCount; i++)
        {
            if (pGameObjects[i].m_allocated)
            {
                renderedEntityCount++;
                GameObject* pGameObject = &pGameObjects[i].m_gameObject;
                const auto* pTransform = pGameObject->GetComponent<Components::Transform>();
                const auto* pModelFilter = pGameObject->GetComponent<Components::ModelFilter>();
                if (!pTransform || !pModelFilter)
                    continue;

                const ResourceSystem::ResourceIndex modelIndex = pModelFilter->GetModel();
                if (modelIndex == ResourceSystem::c_invalidResourceIndex)
                    continue;

                glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), pTransform->m_position) *
                    glm::mat4_cast(pTransform->m_rotation) * glm::scale(glm::mat4(1.0f), pTransform->m_scale);
                glm::mat4 MVP = VP * modelMatrix;

                const Model& model = resourceSystem.GetModel(modelIndex);
                const std::vector<Model::Mesh> meshes = model.GetMeshes();

                Shader::Feature pShaderFeatures[Shader::Feature_Count];
                GLint pShaderFeatureUniforms[Shader::Feature_Count];
                for (const auto& mesh : meshes)
                {
                    if (mesh.m_materialIndex == ResourceSystem::c_invalidResourceIndex)
                        continue;
                    const Material& material = resourceSystem.GetMaterial(mesh.m_materialIndex);
                    const Shader& shader = resourceSystem.GetShader(material.GetShaderIndex());

                    glBindVertexArray(mesh.m_vao);
                    glUseProgram(shader.GetShaderProgram());

                    const size_t shaderFeatureCount = shader.GetFeatureCount();
                    std::fill_n(pShaderFeatures, Shader::Feature_Count, Shader::Feature_invalid);
                    shader.GetFeatures(pShaderFeatures, Shader::Feature_Count);
                    shader.GetFeatureUniformLocations(pShaderFeatureUniforms, Shader::Feature_Count);

                    for (size_t shaderFeatureIndex = 0; shaderFeatureIndex < shaderFeatureCount; shaderFeatureIndex++)
                    {
                        switch (pShaderFeatures[shaderFeatureIndex])
                        {
                            case Shader::Feature_mvp:
                                glUniformMatrix4fv(pShaderFeatureUniforms[shaderFeatureIndex], 1, false, glm::value_ptr(MVP));
                                break;
                            default:
                                break;
                        }
                    }

                    const std::vector<Shader::Property> shaderProperties = shader.GetProperties();
                    const std::vector<Material::PropertyValue>& properties = material.GetProperties();
                    GLint currentTexture = 0;
                    for (size_t shaderPropertyIndex = 0; shaderPropertyIndex < shaderProperties.size(); shaderPropertyIndex++)
                    {
                        const Shader::Property& shaderProperty = shaderProperties[shaderPropertyIndex];
                        switch (shaderProperty.m_propertyType)
                        {
                            case Shader::PropertyType_tex2D:
                                glUniform1i(shaderProperty.m_uniformLocation, currentTexture);
                                glActiveTexture(GL_TEXTURE0 + currentTexture);
                                glBindTexture(GL_TEXTURE_2D, resourceSystem.GetTexture(properties[shaderPropertyIndex].m_resourceIndex).GetTexture());
                                currentTexture++;
                                break;
                            default:
                                break;
                        }
                    }
                    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.m_indexCount), GL_UNSIGNED_INT, nullptr);
                }
            }
        }
    }
}