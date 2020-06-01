////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - editor_model.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QObject>

/**
* Allows run time editing of shaders
*/
class EditorModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList shaders READ Shaders NOTIFY ShadersChanged)
    Q_PROPERTY(int shaderIndex READ ShaderIndex WRITE SetShaderIndex NOTIFY ShaderIndexChanged)
    Q_PROPERTY(QString shaderText READ ShaderText NOTIFY ShaderTextChanged)
    Q_PROPERTY(QString shaderAssembly READ ShaderAssembly NOTIFY ShaderAssemblyChanged)

public:

    using QObject::QObject;

    /**
    * Sets the available shaders for the combo box
    * @param selected The initially selected shader
    * @param shaders The shader names to set
    */
    void InitialiseShaders(int selected, const std::vector<std::string>& shaders);

    /**
    * @return whether the combo box is filled or not
    */
    bool HasShaders() const;

    /**
    * Property getter for shaders list
    */
    const QStringList& Shaders() const;

    /**
    * Property setter/getter for selected shader
    */
    void SetShaderIndex(int index);
    int ShaderIndex() const;

    /**
    * Property setter/getter for the selected shader's text
    */
    void SetShaderText(const QString& text);
    const QString& ShaderText() const;

    /**
    * Property setter/getter for the selected shader's assembly
    */
    void SetShaderAssembly(const QString& assembly);
    const QString& ShaderAssembly() const;

    /**
    * Sends the current changes to the render engine to be compiled
    * @param text New text to compile the shader to
    */
    Q_INVOKABLE void CompileSelectedShader(const QString& text);

signals:

    void RequestCompileSelectedShader(const QString& text);
    void ShaderIndexChanged(int index);
    void ShadersChanged();
    void ShaderAssemblyChanged();
    void ShaderTextChanged();

private:

    int m_shaderIndex = -1;    ///< Currently selected shader
    QStringList m_shaders;     ///< List of editable shaders
    QString m_shaderText;      ///< Text for the selected shader
    QString m_shaderAssembly;  ///< Assembly for the selected shader
};
