#include <iostream>
#include "../src/UTF8Lite/utf8lite.h"

int main(int argc, char *argv[]) {
  std::cout << "Test LiteUTF8" << std::endl;
  std::string str = "Смешанная string for the test (Latin || Кирилица && Latin)";
  std::cout << "Тестовая строка: "  << str << std::endl;
  std::cout << "Количество символов в строке" << std::endl;
  std::cout <<  Utf8SizeStr(str) << std::endl;
  std::cout << "Обрезка UTF-8 строки до 29 символов" << std::endl;
  std::cout <<  Utf8SubStr(str, 29) << std::endl;
  std::cout << "Поиск подстроки 'Latin'  в строке с 0 позиции" << std::endl;
  std::cout <<  Utf8FoundStr(str, "Latin", 0) << std::endl;
  std::cout << "Поиск подстроки 'Latin'  в строке с 40 позиции" << std::endl;
  std::cout <<  Utf8FoundStr(str, "Latin", 40) << std::endl;
  std::cout << "Удаление 9 символов в строке c 31 позиции" << std::endl;
  std::cout <<  Utf8EraseStr(str, 31, 9) << std::endl;
  std::cout << "Вставка строки  (Test_Тест )в исходную строку с 30 позиции" << std::endl;
  std::cout <<  Utf8InsertStr(str, "Test_Тест ", 30) << std::endl;
  std::cout << "Замена подстроки 'Latin' на строку 'Латиница' в строке" << std::endl;
  std::cout <<  Utf8ReplaceStr(str, "Latin", "Латиница") << std::endl;
  str = "StringСтрока";
  std::cout << "Выравнивание строки: " << str << std::endl;
  std::cout << "По левому краю" << std::endl;
  std::cout << "#" << UTF8AlignmentStr(str, 30, Alignment::left) << "#" << std::endl;
  std::cout << "По центру" << std::endl;
  std::cout << "#" << UTF8AlignmentStr(str, 30, Alignment::center) << "#" << std::endl;
  std::cout << "По правому краю" << std::endl;
  std::cout << "#" << UTF8AlignmentStr(str, 30, Alignment::right) << "#" << std::endl;
  str = "String";
  std::string optStr = "Строка";
  std::cout << "Выравнивание строки: '" << str << "' по правому краю, '" << optStr
                                        << "' по левому" << std::endl;
  std::cout << "#" << UTF8AlignmentStr(str, 30, Alignment::leftRight, optStr) << "#" << std::endl;
  return 0;
}
