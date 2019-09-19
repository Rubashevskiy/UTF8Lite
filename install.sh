#!/bin/bash
#Название проекта
project="UTF8Lite"
#Каталог проекта
path=$PWD
#Каталог сборки
build="build"
#Расположение CmakeList.txt
cmakeList="cmake"

echo "Build test example : ${project}"
echo "Path build project : ${path}/${build}"

function dialogYNX () {
  if ([ ! -n "$1" ]); then 
    echo "Error dialogYNX: param is empty"
    exit 1
  fi
  while [ 1 = 1 ]; 
  do
    read -p "$1 (y - Yes, n - No x - Exit) " -n 1 -r
    if [[ $REPLY =~ ^[Yy]$ ]]; then
      echo
      return 0
    fi
    if [[ $REPLY =~ ^[Nn]$ ]]; then
      echo
      return 1
    fi
    if [[ $REPLY =~ ^[Xx]$ ]]; then
      echo "Exit"
      exit
    fi
    echo
  done
}

#Проверка наличия каталога, если нет создание.
#Аргументы: Полный путь к каталогу.
#Возвращает 0 - Успешно || 1 - Ошибка'
function foundOrCreateDir () {
  if ([ ! -n "$1" ]); then 
    echo "Error foundOrCreateDir: param is empty"
    exit 1
  fi
  if [ -d $1 ]; then
    return 0
  else
    if ( mkdir -p $1 ); then
      return 0
    else 
      echo "Error: create dir $1"
      return 1
    fi
  fi
}

#Проверка наличия файла.
#Аргументы: Полный путь к файлу.
#Возвращает 0 - Есть || 1 - Нет'
function isFile () {
  if (! [ -n "$1" ]); then
    echo "Error isFile: param is empty"
    exit 1
  fi
  if ( [ -e $1 ] ); then
    return 0
  else 
    return 1
  fi
}

#Проверка наличия приложения.
#Аргументы: Имя приложения.
#Возвращает 0 - Есть || 1 - Нет
function isApp () {
  if ( ! [ -n "$1" ]); then
    echo "Error isApp: param is empty"
    exit 1
  fi
  if ([ -x "$(command -v $1)" ]); then
    return 0
  else
    return 1
  fi
}

#Проверка и подготовка субмодулей
function FoundAddiction () {
  #Поиск файла с зависимостями
  if (! isFile "${path}/.gitmodules"); then
    echo "Addiction not found OK"
    return 0
  fi
  # Инициализация  локального файла конфигурации и получение всех данных из подмодуля
  if (! git submodule init) || (! git submodule update); then
    echo "Error FoundAddiction: submodule init or submodule update"
    return 1
  else
    echo "Addiction download OK"
    return 0
  fi
}

#Сборка тестового проекта
function buildProject() {
  if ([ ! -n "$1" ]) || ([ ! -n "$2" ]); then 
    echo "Error buildProject: param is empty"
    exit
  fi
  #Переход в каталог сборки
  cd $1
  #Генерируем Makefile и собираем проект
  if (cmake $2) && (make); then
    echo "Buil test project OK"
  else
    echo "Buil test project ERROR"
    exit
  fi
  # Возврат в корень каталога
  cd ${path}
}

#Запуск тестового придлжения
function runProject () {
  if (dialogYNX "Run test project?"); then
    if (${path}/${build}/${project}); then
      echo "Test App OK"
      return 1
    else 
      echo "Test App error"
      return 0
    fi
  fi
}
# Проверка существования папки build в корне проекта, если нет создаем
if (! foundOrCreateDir "${path}/${build}/"); then
  exit
fi
# Проверка зависимостей и их выкачка в случае необходимости
if (! FoundAddiction ); then
  exit 1
fi

# Сборка тестового проекта
buildProject "${path}/${build}/" "${path}/${cmakeList}/"
# Запус тестового приложения по желанию пользователя
runProject
# Эникей
read -n 1 -s -r -p "Press any key to exit"
echo
