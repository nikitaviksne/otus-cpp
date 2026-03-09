#include <iostream>
#include <cstdint>
#include <gtest/gtest.h>  
#include <sstream> 
#include "editor.h"

TEST(TestEDITOR, rectDraw) {
	std::stringstream buffer;
	std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    	EditorController editor;
    	onNewDocumentClick(editor);
    	std::cout.rdbuf(old); // Восстанавливаем оригинальный stdout
    	EXPECT_EQ(buffer.str(), "New document created.\n"); // Проверяем захваченный выв
}

TEST(TestEDITOR, circDraw) {
	std::stringstream buffer;
	std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    	EditorController editor;
    	onAddCircleClick(editor);
    	std::cout.rdbuf(old); // Восстанавливаем оригинальный stdout
    	EXPECT_EQ(buffer.str(), "--- Rendering Canvas ---\nDrawing Circle\n"); // Проверяем захваченный выв
}
TEST(TestEDITOR, Export) {
	std::stringstream buffer;
	std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    	EditorController editor;
    	editor.exportToFile("export.vec");
    	std::cout.rdbuf(old); // Восстанавливаем оригинальный stdout
    	EXPECT_EQ(buffer.str(), "Exporting to export.vec...\n"); // Проверяем захваченный выв
}
TEST(TestEDITOR, Import) {
	std::stringstream buffer;
	std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    	EditorController editor;
    	editor.importFromFile("import.vec");
    	std::cout.rdbuf(old); // Восстанавливаем оригинальный stdout
    	EXPECT_EQ(buffer.str(), "Importing from import.vec...\n"); // Проверяем захваченный выв
}

int main(int nArgs, char** vArgs) {
    ::testing::InitGoogleTest(&nArgs, vArgs);
    return RUN_ALL_TESTS(); 
}
