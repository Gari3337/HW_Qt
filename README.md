# COMPUTER ACCOUNTIN APPLICATION

_____

## ОПИСАНИЕ
Этот проект представляет собой программу для учета компьюторов, с помощью которой можно будет добавлять, удалять, а также редактировать и искать компьютеры.

_____

## ТРЕБОВАНИЯ

+ Docker

____

## СТРУКТУРА ПРОЕКТА 

+ CMakeLists.txt — конфигурационный файл CMake для сборки проекта.
+ Исходные файлы (.cpp, .hpp) — реализация программы.
+ Dockerfile — инструкция для создания Docker-образа.
+ computers.json — файл для хранения обработанных данных.

## Сборка и запуск с использованием Docker

1. Клонируйте репозиторий 
```bash
git clone https://github.com/Gari3337/HW_Qt
```
2. Собираем Docker-образ
```bash
docker build -t apps_for_computers .
```
3. Запуск контейнера
```
docker run --rm -v $(pwd)/Computer_app:/Computer_app apps_for_computers
```
4. Заходим в директорию Computer_app
```
cd Computer_app
```
5. Даем права на выполнение
```
chmod +x Apps_for_computers
```
6. Запускаем приложение (нужны права суперпользователя) 
```
sudo ./Apps_for_computers
```




