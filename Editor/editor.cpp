#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include "editor.h"

// --- MODEL ---

// Базовый класс для графических примитивов (Полиморфизм)
//Shape::~Shape() = default;
//void Shape::draw() const = 0;

void Rectangle::draw() const { std::cout << "Drawing Rectangle\n"; }

void Circle::draw() const { std::cout << "Drawing Circle\n"; }

// Класс документа (Хранилище данных)
void Document::addShape(std::shared_ptr<Shape> shape) {
        shapes.push_back(shape);
    }
    
    void Document::removeShape(size_t index) {
        if (index < shapes.size()) {
            shapes.erase(shapes.begin() + index);
        }
    }

    const std::vector<std::shared_ptr<Shape>>& Document::getShapes() const {
        return shapes;
    }

    void Document::clear() { shapes.clear(); }

// --- VIEW ---

void Canvas::render(const Document& doc) const {
        std::cout << "--- Rendering Canvas ---\n";
        for (const auto& shape : doc.getShapes()) {
            shape->draw();
        }
    }

// --- CONTROLLER ---

    EditorController::EditorController() : model(std::make_unique<Document>()), view(std::make_unique<Canvas>()) {}

    void EditorController::createNewDocument() {
        model->clear();
        std::cout << "New document created.\n";
    }

    void EditorController::importFromFile(const std::string& filename) {
        std::cout << "Importing from " << filename << "...\n";
        // Логика загрузки
    }

    void EditorController::exportToFile(const std::string& filename) {
        std::cout << "Exporting to " << filename << "...\n";
        // Логика сохранения
    }

    void EditorController::addPrimitive(std::shared_ptr<Shape> shape) {
        model->addShape(shape);
        updateView();
    }

    void EditorController::removePrimitive(size_t index) {
        model->removeShape(index);
        updateView();
    }

    void EditorController::updateView() const {
        view->render(*model);
    }

// --- GUI HANDLERS---

void onNewDocumentClick(EditorController& ctrl) {
    ctrl.createNewDocument();
}

void onAddCircleClick(EditorController& ctrl) {
    ctrl.addPrimitive(std::make_shared<Circle>());
}

void onRemoveClick(EditorController& ctrl, size_t index) {
    ctrl.removePrimitive(index);
}
