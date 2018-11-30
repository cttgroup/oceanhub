# dump
from django.db import models


class DumpTask(models.Model):
    timestamp = models.DateTimeField(
        auto_now_add=True)
    id = models.CharField('ID задачи',
        primary_key=True,
        max_length=50,
        )
    STATUS = (
        ('-1', 'Ошибка'),
        ('0', 'Новая задача'),
        ('1', 'Идёт обработка'),
        ('2', 'Готово'),
    )
    status = models.CharField('Статус выполнения задачи',
        max_length=2,
        choices=STATUS,
        default='0',
        )
    file_name = models.CharField('Путь к файлy с дампом',
        max_length=200,
        blank=True,
        default='',
        )
    file_size = models.CharField('Размер файла',
        max_length=50,
        blank=True,
        default='',
        )
    elapsed_time = models.CharField('Время выполнения, секунды',
        max_length=10,
        blank=True,
        default='',
        )

    def __str__(self):
        return str(self.id) \
            + '-' \
            + str(self.timestamp.strftime('%H:%M:%S-%Y%m%d'))
