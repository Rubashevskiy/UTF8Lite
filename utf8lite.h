#ifndef UTF8LITE_H
#define UTF8LITE_H

#include <string>

enum class Alignment {
  left,      ///< По левому края
  center,    ///< По центру
  right,     ///< По правому краю
  leftRight  ///< Часть по левому часть по правому
};

template<typename iterator1, typename iterator2>
  inline size_t IncUtf8StringIterator(iterator1& it, const iterator2& last) {
    if (it == last) {
    return 0;
  }
  size_t result = 1;
  for (++it; last != it; ++it, ++result) {
    unsigned char c = *it;
    if(!(c& 0x80) || ((c& 0xC0) == 0xC0)) {
      break;
    }
  }
  return result;
}

template<typename iterator>
inline size_t MultIncUtf8StringIterator(iterator& it, const iterator& last, size_t count) {
  size_t result = 0;
  for(size_t i = 0; i < count; i++) {
    if(it == last) {
      break;
    }
    result += IncUtf8StringIterator(it, last);
  }
  return result;
}

template<typename Iterator1>
inline Iterator1 GetMultIncUtf8StringIterator(Iterator1 it, const Iterator1& last, size_t count) {
  MultIncUtf8StringIterator(it, last, count);
  return it;
}

inline std::string::const_iterator Utf8PositionToIterator(const std::string& str, size_t pos) {
  std::string::const_iterator result = str.begin();
  MultIncUtf8StringIterator(result, str.end(), pos);
  return result;
}

inline std::string::iterator Utf8PositionToIterator(std::string& str, size_t pos) {
  std::string::iterator result = str.begin();
  MultIncUtf8StringIterator(result, str.end(), pos);
  return result;
}

/*!
 * @brief Подсчет количества символов в UTF-8 строке
 * @param str     Строка
 * @return size_t Размер
 */
inline size_t Utf8SizeStr(const std::string& str) {
  size_t result = 0;
  std::string::const_iterator it = str.begin();
  for(; it != str.end(); IncUtf8StringIterator(it, str.end())) {
    result++;
  }
  return result;
}

/*!
 * @brief Обрезка UTF-8 строки до нужного размера
 * @param str          Исходная строка
 * @param start        Начальная позиция
 * @param count        Размер результирующих символов 
 * @return std::string Обрезаная строка
 */
inline std::string Utf8SubStr(const std::string& str, size_t count = (size_t)-1, size_t start = 0) {
  if (count == (size_t)-1) {
    return std::string(Utf8PositionToIterator(str, start), str.end());
  }
  else {
    return std::string(Utf8PositionToIterator(str, start), Utf8PositionToIterator(str, start + count));
  }
}

/*!
 * @brief Поиск подстроки found в строке str с указанной позиции
 * @param str     Строка источник
 * @param found   Строка для поиска 
 * @param pos     Позиция
 * @return size_t Если найденна возвращает обсолютную позицию в строке, нет возвращает std::string::npos
 */
inline size_t Utf8FoundStr(const std::string& str, const std::string& found, size_t pos = 0) {
  for (int i = 0; i < Utf8SizeStr(str); i++) {
    if (found == std::string(Utf8PositionToIterator(str, pos + i),
                             Utf8PositionToIterator(str, pos + Utf8SizeStr(found) + i))) {
      return pos + i;
    }
  }
  return std::string::npos;
}

/*!
 * @brief Удаление символов в строке
 * @param str   Исходная строка
 * @param start Позиция с которой производится удаление
 * @param n     Количество символов для удаления
 * @return      Результатирующая строка
 */
inline std::string Utf8EraseStr(const std::string& str, size_t pos, size_t count = (size_t)-1) {
  std::string result{str};
  std::string::iterator it = Utf8PositionToIterator(result, pos);
  result.erase(it, GetMultIncUtf8StringIterator(it, result.end(), count));
  return result;
}

/*!
 * @brief Вставка строки в исходную строку с нужной позиции
 * @param str   Исходная строка
 * @param s     Строка для вставки
 * @param start Позиция с которой производится вставка
 * @return      Результатирующая строка
 */
inline std::string Utf8InsertStr(const std::string& str, const std::string& s, size_t pos = 0 ) {
  std::string result{str};
  result.insert(Utf8PositionToIterator(result, pos), s.begin(), s.end());
  return result;
}

/*!
 * @brief Замена подстроки в строке
 * @param str      Исходная строка
 * @param foundStr Строка которую заменяем
 * @param replStr  Строка для замены
 * @return         Результатирующая строка
 */
inline std::string Utf8ReplaceStr(const std::string& str, const std::string& foundStr, const std::string& replStr) {  
  if (foundStr == replStr) {
    return str;
  }
  std::string result{str};
  size_t sPos = 0;
  while (sPos < Utf8SizeStr(result)) {
    size_t found = Utf8FoundStr(result, foundStr, sPos);
    if (found != std::string::npos) {
      result = Utf8EraseStr(result, found, Utf8SizeStr(foundStr));
      result = Utf8InsertStr(result, replStr, found);
      sPos += Utf8SizeStr(foundStr);
    }
    else {
      break;
    }
  }
  return result;
}

/*!
 * @brief Выравнивание подстроки в строке
 * 
 * @param str          Строка для выравнивания
 * @param newSize      Размер результатирующей строки
 * @param alignment    Выравнивание
 * @param optStr       Дополнительная строка для leftRight выравнивания
 * @return std::string Результатирующая строка
 */
inline std::string UTF8AlignmentStr(const std::string& str,
                          const size_t& newSize, const Alignment& alignment, const std::string& optStr = "") {
  size_t x = Utf8SizeStr(str);
  if (x > newSize) {
    std::string test = Utf8SubStr(str, newSize, 0);
    return test;
  }

  std::string result{};
  switch (alignment) {
    case Alignment::left:
    {
      result = str + std::string(newSize - Utf8SizeStr(str) , ' ');
      break;
    }
    case Alignment::center:
    {
      int sFirst = (newSize/2) - (Utf8SizeStr(str)/2);
      int sEnd = newSize - sFirst - Utf8SizeStr(str);
      result = std::string(sFirst , ' ') + str + std::string(sEnd , ' ');
      break;
    }
    case Alignment::right:
    {
      result = std::string(newSize - Utf8SizeStr(str) , ' ') + str;
      break;
    }
    case Alignment::leftRight:
    {
      int fullWidth = Utf8SizeStr(str) + Utf8SizeStr(optStr) +1;
      if (fullWidth > newSize) {
        result = Utf8SubStr(str + " " + optStr, newSize, 0);
        break;
      }
      int sSize = newSize - Utf8SizeStr(str) - Utf8SizeStr(optStr);
      result =  str + std::string(sSize , ' ') + optStr;
      break;
    }
  }
  return result;
}

#endif //UTF8LITE_H

