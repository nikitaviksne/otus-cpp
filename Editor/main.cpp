#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include "editor.h"


int main() {
    EditorController editor;

    // Имитация работы пользователя с GUI
    onNewDocumentClick(editor);
    onAddCircleClick(editor);
    editor.addPrimitive(std::make_shared<Rectangle>());
    
    editor.exportToFile("project.vec");
    
    onRemoveClick(editor, 0);

    return 0;
}

