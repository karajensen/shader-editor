////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - editor_model.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "editor_model.h"
#include "stringlist_model.h"

EditorModel::EditorModel(QObject* parent)
    : QObject(parent)
    , m_shadersModel(new StringListModel(this))
{
}

void EditorModel::SetShaderText(const QString& text)
{
    if (m_shaderText != text)
    {
        m_shaderText = text;
        emit ShaderTextChanged();
    }
}

const QString& EditorModel::ShaderText() const
{
    return m_shaderText;
}

void EditorModel::SetShaderAssembly(const QString& assembly)
{
    if (m_shaderAssembly != assembly)
    {
        m_shaderAssembly = assembly;
        emit ShaderAssemblyChanged();
    }
}

const QString& EditorModel::ShaderAssembly() const
{
    return m_shaderAssembly;
}

StringListModel* EditorModel::ShadersModel() const
{
    return m_shadersModel;
}

void EditorModel::CompileSelectedShader(const QString& text)
{
    emit RequestCompileSelectedShader(text);
}
