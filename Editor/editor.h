#include <iostream> 
#include <vector>
#include <memory>
#include <string>
#include <algorithm>

// --- MODEL ---

// Базовый класс для графических примитивов (Полиморфизм)
class Shape {
public:
    virtual ~Shape() = default;
    virtual void draw() const= 0;
};

class Rectangle : public Shape {
public:
    void draw() const override;
};

class Circle : public Shape {
public:
    void draw() const override;
};

// Класс документа (Хранилище данных)
class Document {
    std::vector<std::shared_ptr<Shape>> shapes;
public:
    void addShape(std::shared_ptr<Shape> shape);
    
    void removeShape(size_t index) ;


    const std::vector<std::shared_ptr<Shape>>& getShapes() const;

    void clear() ;
};

// --- VIEW ---

class Canvas {
public:
    void render(const Document& doc) const;
};

// --- CONTROLLER ---

class EditorController {
    std::unique_ptr<Document> model;
    std::unique_ptr<Canvas> view;

public:
    EditorController();

    void createNewDocument();

    void importFromFile(const std::string& filename);

    void exportToFile(const std::string& filename);

    void addPrimitive(std::shared_ptr<Shape> shape);

    void removePrimitive(size_t index);

    void updateView() const;
};

// --- GUI HANDLERS  ---

void onNewDocumentClick(EditorController& ctrl);

void onAddCircleClick(EditorController& ctrl);

void onRemoveClick(EditorController& ctrl, size_t index);

