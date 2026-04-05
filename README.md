# Web-Agent :smile:

## Описание:
Web-Agent — это кроссплатформенный C++ агент, который связывает веб-интерфейс с локальными программами на клиентской машине. При нажатии кнопки в веб-приложении агент запускает нужную программу, обрабатывает данные и отправляет результат на сервер.
Проект демонстрирует применение современных возможностей C++: шаблоны, многопоточность, RAII, а также интеграцию популярных библиотек.

## Функционал:
Веб-агент регистрируется на сервере, периодически опрашивает его на наличие задач, получает инструкции, запускает указанные программы на локальном компьютере, собирает результаты выполнения (код возврата, вывод, файлы) и отправляет их обратно на сервер. Все действия записываются в лог. Поддерживается многопоточное выполнение задач и работа на Windows, Linux и macOS.

## Поддерживаемые платформы
Проект WEB-AGENT является полностью кроссплатформенным и поддерживает следующие операционные системы:
Windows (Windows 10 / Windows 11)
Linux (Ubuntu 20.04+, Debian 11+, Fedora 38+, CentOS 8+, RHEL 8+, Arch Linux)
macOS (macOS 12 (Monterey) и новее)

## Требования:
* C++17
* CMake 3.15+
* libssl-dev, zlib1g-dev

## Установка зависимостей :
1. Для macOS:
brew install cmake openssl
2. Для Ubuntu/Debian:
sudo apt update
sudo apt install build-essential cmake libssl-dev
3. Для Fedora, CentOS, RHEL и производных:
sudo dnf install gcc-c++ cmake openssl-devel meson ninja-build
4. Для Arch Linux:
sudo pacman -S gcc cmake openssl meson
Для Windows:
Следуйте инструкциям в официальной документации:
* CMake
* OpenSSL
* Meson
## Сборка проекта:
```
mkdir -p build && cd build && cmake .. && cmake --build .
```
### Запуск программы:
` ./web_agent `
## Используемые технологии
* C++17
* CMake
* OpenSSL
* CPR — HTTP-клиент
* nlohmann/json
* spdlog
* Google Test
* REST API
