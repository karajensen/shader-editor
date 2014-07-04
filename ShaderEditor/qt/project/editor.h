////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - editor.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>

#ifdef _VS
#include "generated/ui_editor.h"
#else
#include "ui_editor.h"
#endif

/**
* Allows run time editing of shaders
* @note Read by both VS and Qt Editor
*/
class Editor : public QWidget
{
    Q_OBJECT

public:

    /**
    * Constructor
    * @param parent The owner of this widget
    */
    Editor(QWidget* parent = nullptr);

private slots:


private:

    Ui::Editor m_ui; ///< User interface object
};
