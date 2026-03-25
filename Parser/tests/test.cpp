#include <iostream>
#include <gtest/gtest.h>  
#include <gmock/gmock.h>  
#include <vector>
#include <ctime>
#include <chrono>
#include <thread>
#include "BulkHandler.h"  
#include "Observer.h"  

using ::testing::_;          // Matcher "любое значение"
using ::testing::Gt;         // Matcher "Greater Than" (больше чем)
using ::testing::AllOf;      // Объединение условий


// Создаем Mock для наблюдателя
class MockObserver : public Observer{
public:
    MOCK_METHOD(void, update, (const std::vector<std::string>& commands, std::time_t time), (override));
};

TEST(BulkHandlerTest, StaticBlockTrigger) {//Проверка вызова метода нужное кол-во раз
    auto mockObs = std::make_shared<MockObserver>();
    BulkHandler handler(2); // N = 2
    handler.addObserver(mockObs);

    // update должен вызваться один раз с вектором из 2 элементов
    EXPECT_CALL(*mockObs, update(testing::ElementsAre("cmd1", "cmd2"), testing::_)).Times(1);

    handler.process("cmd1");
    handler.process("cmd2"); // Здесь должен сработать flush
}


TEST(BulkHandlerTest, DynamicBlockIncompleteIgnore) {//провека невызова вложенного блока
    auto mockObs = std::make_shared<MockObserver>();
    BulkHandler handler(3);
    handler.addObserver(mockObs);

    // только первый блок
    EXPECT_CALL(*mockObs, update(testing::ElementsAre("cmd1"), testing::_)).Times(1);

    handler.process("cmd1");
    handler.process("{");
    handler.process("cmd2");
    handler.process("cmd3");
    //завершение программы без закрывающей скобки
    handler.forceFlush(); 
}

//Время только лишь первой комманды
TEST(BulkHandlerTest, FirstCommandTimestampTest) {
    auto mockObs = std::make_shared<MockObserver>();
    BulkHandler handler(2);
    handler.addObserver(mockObs);

    // время "сейчас"
    std::time_t startTime = std::time(nullptr);

    // ожидание:
    // аргумент (time) >= startTime
    EXPECT_CALL(*mockObs, update(_, Gt(startTime - 1))).Times(1); //Gt(startTime - 1)

    // задержкa
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // вывод команд
    handler.process("cmd1"); 
    std::this_thread::sleep_for(std::chrono::seconds(1));
    handler.process("cmd2");
}

TEST(BulkHandlerTest, TimestampIsFromFirstCommand) {
    auto mockObs = std::make_shared<MockObserver>();
    BulkHandler handler(2);
    handler.addObserver(mockObs);

    handler.process("cmd1");
    std::time_t timeAtFirstCmd = std::time(nullptr);
    
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Проверяем, что время в логгере в точности равно времени первой команды
    EXPECT_CALL(*mockObs, update(_, timeAtFirstCmd)).Times(1);
    
    handler.process("cmd2");
}

int main(int nArgs, char** vArgs) {
    ::testing::InitGoogleTest(&nArgs, vArgs);
    return RUN_ALL_TESTS(); 
}
