# Button Box (Arduino Pro Micro + 4x4 Keypad)

## Подключение

- `R1 -> 2`
- `R2 -> 3`
- `R3 -> 4`
- `R4 -> 5`
- `C1 -> 6`
- `C2 -> 7`
- `C3 -> 8`
- `C4 -> 9`

## Прошивка

- Сборка: `pio run`
- Заливка: `pio run -t upload`
- Список портов: `pio device list`
- Монитор порта (опционально): `pio device monitor --port COMXX --baud 115200`

## Windows: переключение мониторов

1. Установить [AutoHotkey v2](https://www.autohotkey.com/).
2. Запустить файл `display-switch.ahk`.

## Автозапуск (Windows)

1. Нажать `Win + R`.
2. Ввести `shell:startup`.
3. Нажать Enter.
4. Добавить в открывшуюся папку ярлык на `display-switch.ahk`.

## Назначение кнопок

- `1` -> `Ctrl+Alt+1` -> `DisplaySwitch /internal` (только экран ПК)
- `2` -> `Ctrl+Alt+2` -> `DisplaySwitch /clone` (дублировать)
- `3` -> `Ctrl+Alt+3` -> `DisplaySwitch /extend` (расширить)
- `A` -> `Ctrl+Alt+4` -> `DisplaySwitch /external` (только внешний экран)