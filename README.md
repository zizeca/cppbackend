# cpp-backend-template

Шаблон репозитория для практического трека «C++ backend».

# Начало работы

Склонируйте репозиторий в любую подходящую директорию на вашем компьютере.

# Обновление шаблона

Чтобы иметь возможность получать обновления автотестов и других частей шаблона выполните следующую команду:

```
git remote add -m main template https://github.com/cpppracticum/cpp-backend-template-practicum.git
```

Для обновления кода автотестов выполните команду:

```
git fetch template && git checkout template/main .github
```

Затем добавьте полученные изменения в свой репозиторий.

# Тестирование в Github Actions

При отправке (push) изменений в ветку main репозитория будет запущен пайплайн Github Actions, 

(например, https://github.com/cpppracticum/cpp-backend-template-practicum/runs/7344792085?check_suite_focus=true#step:6:25),

который позволит увидеть результат прохождения тестов.

Пример прошедшего пайплайна:
https://github.com/cpppracticum/cpp-backend-template-practicum/runs/7344821341?check_suite_focus=true#step:6:16.
