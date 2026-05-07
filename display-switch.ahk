#Requires AutoHotkey v2.0
#SingleInstance Force

; 1 = Only PC screen
^!1:: Run("DisplaySwitch.exe /internal")

; 2 = Duplicate
^!2:: Run("DisplaySwitch.exe /clone")

; 3 = Extend
^!3:: Run("DisplaySwitch.exe /extend")

; 4 = Second screen only
^!4:: Run("DisplaySwitch.exe /external")
