#include "ShaderManager.h"
namespace NCL
{
    namespace Rendering
    {
        ShaderManager* ShaderManager::m_Instance = nullptr;
        ShaderManager::ShaderManager()
        {
            m_Init = false;
            m_Shaders.insert({ "default", new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl") });

            for (auto& shader : m_Shaders)
            {
                if (!(shader.second->LoadSuccess()))
                {
                    return;
                }
            }
            m_Init = true;
        }
        ShaderManager* ShaderManager::GetInstance()
        {           
            return m_Instance;
        }

        void ShaderManager::Init()
        {
            m_Instance = new ShaderManager();
        }

        void ShaderManager::Finish()
        {
            delete m_Instance;
        }

        OGLShader* ShaderManager::GetShader(const char* name)
        {
            return m_Shaders.at(name);
        }

        ShaderManager::~ShaderManager()
        {
            for (auto& shader : m_Shaders)
            {
                delete shader.second;
            }
        }
    }
};