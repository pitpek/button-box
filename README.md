# Button box

## Быстрый старт

1. Установи PlatformIO в VS Code / через CLI.
2. Подключи Pro Micro по USB (выбери правильный порт в PlatformIO).
3. Собрать проект:
   - `pio run`
4. Загрузить (подставь свой порт при необходимости):
   - `pio run -t upload`
5. Монитор:
   - `pio device monitor --port COM13 --baud 115200`