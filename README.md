# COMPUTER ACCOUNTIN APPLICATION

_____

## ОПИСАНИЕ
Этот проект представляет собой программу для учета компьюторов, с помощью которой можно будет добавлять, удалять, а также редактировать и искать компьютеры.

_____

## ТРЕБОВАНИЯ

+ ОС: Linux подобная система
+ Docker
____

## СТРУКТУРА ПРОЕКТА 

+ CMakeLists.txt — конфигурационный файл CMake для сборки проекта.
+ Исходные файлы (.cpp, .hpp) — реализация программы.
+ Dockerfile — инструкция для создания Docker-образа.
+ computers.json — файл для хранения обработанных данных.

## Сборка и запуск с использованием Docker

1. Скачайте необходимую библиотеку для использования GUI на хосте.
```
sudo apt install libqt5widgets5
```
2. Клонируйте репозиторий 
```bash
git clone https://github.com/Gari3337/HW_Qt
```
3. Перейдите в загруженную директорию
```
cd HW_Qt
```
4. Собираем Docker-образ
```bash
sudo docker build -t apps_for_computers .
```
5. Запуск контейнера
```
sudo docker run --rm -v $(pwd)/Computer_app:/Computer_app apps_for_computers
```
6. Заходим в директорию Computer_app
```
cd Computer_app
```
7. Даем права на выполнение
```
sudo chmod +x Apps_for_computers
```
8. Запускаем приложение (нужны права суперпользователя) 
```
sudo ./Apps_for_computers
```




