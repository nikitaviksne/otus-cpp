#include <iostream>
#include <gtest/gtest.h>  

#if 0
TEST_F(DuplicateFinderTest, BlockHashingWithPadding) {
    CreateFile("test.txt", "Hello"); // 5 байт
    FileState fs_obj(test_dir / "test.txt");

    size_t S = 8; // Размер блока больше файла
    auto hash1 = fs_obj.get_block_hash(0, S);
    
    // Проверим, что повторный вызов дает тот же хеш (из кеша)
    auto hash2 = fs_obj.get_block_hash(0, S);
    EXPECT_EQ(hash1, hash2);
}

TEST_F(DuplicateFinderTest, IdentifiesDuplicates) {
    CreateFile("f1.txt", "DuplicateContent");
    CreateFile("f2.txt", "DuplicateContent");
    CreateFile("f3.txt", "UniqueContent");

    std::vector<FileState> group;
    group.emplace_back(test_dir / "f1.txt");
    group.emplace_back(test_dir / "f2.txt");
    group.emplace_back(test_dir / "f3.txt");

    std::vector<std::vector<FileState*>> res = process_group(group, 4); // S=4 байта

    std::stringstream ss;
    print_out(res, ss);
    std::string output = ss.str();
    // Проверяем, что f1 и f2 в выводе есть, а f3 нет
    EXPECT_TRUE(output.find("f1.txt") != std::string::npos);
    EXPECT_TRUE(output.find("f2.txt") != std::string::npos);
    EXPECT_TRUE(output.find("f3.txt") == std::string::npos);
}
#endif

int main(int nArgs, char** vArgs) {
    ::testing::InitGoogleTest(&nArgs, vArgs);
    return RUN_ALL_TESTS(); 
}
