////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - editor.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "qt/tweakable.h"

#include <QtCore/qobject.h>

struct SignalCallbacks;

/**
* Allows run time editing of shaders
*/
class Editor : public QObject
{
    Q_OBJECT

public:

    /**
    * Constructor
    * @param callbacks Functions called when a tweakable value emits a signal
    */
    Editor(SignalCallbacks& callbacks);

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
    * Sets the editable shader text for the selected shader
    * @param text The text to edit
    */
    void SetShaderText(const std::string& text);

    /**
    * Sets the readonly shader assembly for the selected shader
    * @param assembly The assembly text
    */
    void SetShaderAssembly(const std::string& assembly);

private slots:
    
    /**
    * Removes any edited changes to the shader text
    */
    void RevertPressed();

    /**
    * Sends the current changes to the render engine to be compiled
    */
    void CompilePressed();

private:

    //TweakableBox m_shaders;        ///< Combo box for holding shaders

    QString m_text;                ///< Text for the selected shader
    QString m_assembly;            ///< Assembly for the selected shader
    SignalCallbacks& m_callbacks;   ///< Callbacks to update the cache
};
