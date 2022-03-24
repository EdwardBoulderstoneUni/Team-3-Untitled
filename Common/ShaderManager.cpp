#include "ShaderManager.h"
#include "RendererBase.h"
#include "Window.h"
namespace NCL
{
    namespace Rendering
    {
        ShaderManager* ShaderManager::m_Instance = nullptr;
        ShaderManager::ShaderManager()
        {
            m_Shaders.insert({ "default", Window::GetRenderer()->load_default_shader() });
            m_Shaders.insert({ "PaintableObjectShader", Window::GetRenderer()->get_paintable_object_shader() });
            m_Shaders.insert({ "PaintInstanceShader", Window::GetRenderer()->get_paintable_instance_shader() });

            for (const auto& shader : m_Shaders)
            {
                if (!(shader.second->LoadSuccess()))
                {
                    return;
                }
            }            
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

        ShaderBase* ShaderManager::GetShader(const char* name) const
        {
            return m_Shaders.at(name);
        }

        void ShaderManager::AddShader(const char* name, ShaderBase* shader)
        {
            m_Shaders.insert({ name, shader });
        }

        ShaderManager::~ShaderManager()
        {
            for (const auto& shader : m_Shaders)
            {
                delete shader.second;
            }
        }
    }
};